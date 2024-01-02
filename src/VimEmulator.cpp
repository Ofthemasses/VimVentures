#include "VimEmulator.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <chrono>
#include <constants.hpp>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <csignal>

VimEmulator::VimEmulator(std::string terminal, std::string nArg) {
    m_display = XOpenDisplay(nullptr);
    m_screen = DefaultScreen(m_display);
    m_rootWindow = RootWindow(m_display, m_screen);
    m_windowName = std::string(APP_TITLE) + "Emulator";
    m_window = nullptr;
    m_modmask = new unsigned int;
    m_frameReady = false;
    m_texture = nullptr;

    // Run the terminal instance
    m_pid = fork();
    if (m_pid == -1){
        std::cerr << "Failed  To Fork VimEmulator" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (m_pid == 0) {
        setenv("DISPLAY", XDisplayString(m_display), 1);
        execlp(terminal.c_str(), terminal.c_str(), nArg.c_str(),
               m_windowName.c_str(),
               "-o", // TODO Replace with "nArgs"
               "window.decorations=None", "-e", "vim", "-S",
               "~/Documents/VimVentures/vim-ventures-autoload.vim", "-c",
               "cd ~/.vimventures", NULL);
        std::cerr << "Terminal Unexpected Exited" << std::endl;
    }
}

VimEmulator::~VimEmulator() {
    kill(m_pid, SIGTERM);
    delete(m_modmask);
}

void VimEmulator::RegisterWindow() {
    std::thread(&VimEmulator::RegisterWindowThread, this).detach();
}

void VimEmulator::ResizeWindow(int w, int h) {
    std::thread(&VimEmulator::ResizeWindowThread, this, w, h).detach();
}

void VimEmulator::QueueFrame() {
    if (m_frameReady) {
        return;
    }
    std::thread(&VimEmulator::QueueFrameThread, this).detach();
}

void VimEmulator::SendSDLKey(SDL_Keycode key) {
    KeySym xkey = SDLX11KeymapRef.convert(key);

    XKeyPressedEvent event = {0};
    event.type = KeyPress;
    event.display = m_display;
    event.window = *m_window;
    event.root = m_rootWindow;
    event.keycode = XKeysymToKeycode(m_display, xkey);
    event.state = 0;
    event.state = *m_modmask;

    XSendEvent(m_display, *m_window, True, KeyPressMask, (XEvent *)&event);
}

void VimEmulator::SetSDLMod(SDL_Keymod mod) { *m_modmask = mod; }

/** IRender **/

void VimEmulator::Render(SDL_Renderer *renderer) {
    if (m_frameReady) {
        SDL_DestroyTexture(m_texture);
        SDL_Surface *surface = this->GetFrameAsSurface();
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        this->QueueFrame();
    }
    if (m_texture == nullptr) {
        return;
    }
    SDL_RenderCopy(renderer, m_texture, nullptr, &m_rect);
};

/** Private Methods **/

SDL_Surface *VimEmulator::GetFrameAsSurface() {
    m_frameReady = false;
    return m_surface;
}

Window *VimEmulator::findWindowByName(Window window) {
    Window parent;
    Window *children;
    unsigned int numChildren;
    char *windowName;

    if (XQueryTree(m_display, window, &window, &parent, &children,
                   &numChildren) != 0) {
        for (unsigned int i = 0; i < numChildren; i++) {
            XFetchName(m_display, children[i], &windowName);
            if ((windowName != nullptr) &&
                strcmp(windowName, m_windowName.c_str()) == 0) {
                return &children[i];
            }
            Window *result = findWindowByName(children[i]);
            if (result != nullptr) {
                return result;
            }
        }
    }
    return nullptr;
}

void VimEmulator::RegisterWindowThread(){
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_xImage = nullptr;
        m_window = nullptr;
    }
    Window *foundWindow = nullptr;
    while (true) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (foundWindow != nullptr) {
            break;
        }
        foundWindow = this->findWindowByName(m_rootWindow);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_MS));
    }
    if (foundWindow != nullptr) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_window = foundWindow;
            XWindowAttributes attributes;
            XGetWindowAttributes(m_display, *m_window, &attributes);
            while (attributes.map_state == 0) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(REFRESH_MS));
                XGetWindowAttributes(m_display, *m_window, &attributes);
            }
            m_width = attributes.width;
            m_height = attributes.height;

            XLowerWindow(m_display, *m_window);
            XSetInputFocus(m_display, m_rootWindow, RevertToPointerRoot, CurrentTime);
            XMoveWindow(m_display, *m_window, -attributes.width * 2 - 1,
                        -attributes.height * 2 - 1);
            XSelectInput(m_display, *m_window, StructureNotifyMask);

            m_xImage = XGetImage(m_display, *m_window, 0, 0, m_width,
                                 m_height, attributes.depth, ZPixmap);
        }
    }
}

void VimEmulator::ResizeWindowThread(int w, int h){
    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_window == nullptr || m_xImage == nullptr) {
                continue;
            }
            m_width = w;
            m_height = h;
            XEvent event;
            while (!this->MatchResizeEvent(w, h, &event)) {
                XResizeWindow(m_display, *m_window, m_width,
                    m_height);
            }
            break;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(REFRESH_MS));
    }
    this->RegisterWindow();
}

bool VimEmulator::MatchResizeEvent(int w, int h, XEvent* event){
    XNextEvent(m_display, event);
    if (event->type == ConfigureNotify) {
        XConfigureEvent xce = event->xconfigure;
        return xce.width == w && xce.height == h;
    }
    return false;
}

void VimEmulator::QueueFrameThread(){
    while (true) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_window != nullptr && m_xImage != nullptr) {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_MS));
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        XGetSubImage(m_display, *m_window, 0, 0, m_width, m_height,
            AllPlanes, ZPixmap, m_xImage, 0, 0);
        m_surface = SDL_CreateRGBSurfaceFrom(
            (void *)m_xImage->data, m_xImage->width, m_xImage->height,
            m_xImage->depth, m_xImage->bytes_per_line, R_MASK, G_MASK,
            B_MASK, A_MASK);
        if (m_surface == nullptr) {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        m_frameReady = true;
    }
}
