#include "VimEmulator.hpp"
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <constants.hpp>
#include <unistd.h>
#include <thread>

VimEmulator::VimEmulator(std::string terminal, std::string nArg){
    m_display = XOpenDisplay(NULL);
    m_screen = DefaultScreen(m_display);
    m_rootWindow = RootWindow(m_display, m_screen);
    m_windowName = std::string(APP_TITLE) + "Emulator";
    m_window = nullptr;
    m_modmask = new unsigned int;
    m_frameReady = false;

    // Run the terminal instance
    pid_t pid = fork();
    if(pid == 0){
        setenv("DISPLAY", XDisplayString(m_display), 1);
        execlp(terminal.c_str(),
                terminal.c_str(),
                nArg.c_str(), 
                m_windowName.c_str(),
                "-o", // TODO Replace with "nArgs"
                "window.decorations=None",
                "-e",
                "vim", 
                "-S", 
                "~/Documents/VimVentures/vim-ventures-autoload.vim",
                "-c", 
                "cd ~/.vimventures",
                NULL);
        exit(EXIT_FAILURE);
    }
}

VimEmulator::~VimEmulator(){}

void VimEmulator::RegisterWindow(){
    m_window = this->findWindowByName(m_rootWindow);

    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, *m_window, &attributes);
    m_width = attributes.width;
    m_height = attributes.height;

    XMoveWindow(m_display, *m_window, -attributes.width*2 - 1, -attributes.height*2 -1);

    m_xImage = XGetImage(m_display,
            *m_window,
            0,
            0,
            m_width,
            m_height,
            AllPlanes,
            ZPixmap);
}

void VimEmulator::ResizeWindow(int w, int h){
    m_width = w;
    m_height = h;
    XResizeWindow(m_display, *m_window, m_width, m_height);
    m_window = this->findWindowByName(m_rootWindow);
    m_xImage = XGetImage(m_display,
            *m_window,
            0,
            0,
            m_width,
            m_height,
            AllPlanes,
            ZPixmap);
}

void VimEmulator::QueueFrame(){
    if (m_frameReady){
        return;
    }
    std::thread([this]() {
  		XGetSubImage(m_display,
  		        *m_window,
  		        0,
  		        0,
                m_width,
                m_height,
  		        AllPlanes,
  		        ZPixmap,
  		        m_xImage,
  		        0,
  		        0);
        m_surface = SDL_CreateRGBSurfaceFrom(
            (void*) m_xImage->data,
            m_xImage->width,
            m_xImage->height,
            m_xImage->depth,
            m_xImage->bytes_per_line,
            (Uint32) 0XFF0000,
            (Uint32) 0X00FF00,
            (Uint32) 0X0000FF,
            0);
        if (m_surface == NULL){
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        m_frameReady = true;
    }).detach();
}

void VimEmulator::SendSDLKey(SDL_Keycode key){
    KeySym xkey = SDLX11KeymapRef.convert(key);

    XKeyPressedEvent event = {0};
    event.type = KeyPress;
    event.display = m_display;
    event.window = *m_window;
    event.root = m_rootWindow;
    event.keycode = XKeysymToKeycode(m_display, xkey);
    event.state = 0;
    event.state = *m_modmask;

    XSendEvent(m_display, *m_window, True, KeyPressMask, (XEvent*)&event);
}

void VimEmulator::SetSDLMod(SDL_Keymod mod){
    *m_modmask = mod;
}

/** IRender **/

void VimEmulator::Render(SDL_Renderer* renderer){
    if (m_frameReady){
        SDL_DestroyTexture(m_texture);
        SDL_Surface* surface = this->GetFrameAsSurface();
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        this->QueueFrame();
    }
    SDL_RenderCopy(renderer, m_texture, NULL, &m_rect);
};

/** Private Methods **/

SDL_Surface* VimEmulator::GetFrameAsSurface(){
    m_frameReady = false;
    return m_surface;
}

Window* VimEmulator::findWindowByName(Window window){
    Window parent;
    Window* children;
    unsigned int numChildren;
    char* windowName;

    if (XQueryTree(m_display, window, &window, &parent, &children, &numChildren)){
        for (unsigned int i = 0; i < numChildren; i++) {
            XFetchName(m_display, children[i], &windowName);
            if (windowName && strcmp(windowName, m_windowName.c_str()) == 0){
                return &children[i];
            }
            Window* result = findWindowByName(children[i]);
            if (result) return result;
        }
    }
    return nullptr;
}
