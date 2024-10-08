#include "VimEmulator.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <chrono>
#include <constants.hpp>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

/**
 * A Vim Terminal Instance.
 *
 * @param terminal The name of the terminal program, E.g. 'Alacritty'
 * @param nArg Arguments for launching vim
 */
VimEmulator::VimEmulator(std::string terminal, std::string nArg) {
    m_display = XOpenDisplay(nullptr);
    m_screen = DefaultScreen(m_display);
    m_rootWindow = RootWindow(m_display, m_screen);
    m_window = nullptr;
    m_windowName = std::string(APP_TITLE) + "Emulator";
    m_modmask = new unsigned int;
    m_frameReady = false;

    m_latestsocket = -1;
    m_requestReady = false;
    m_recievingBuffer = false;
    InitializeTCPLayer();

    // Run the terminal instance
    m_pid = fork();
    if (m_pid == -1) {
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

/**
 * Destroys the terminal and pointer vars.
 */
VimEmulator::~VimEmulator() {
    if (m_display != nullptr) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_window != nullptr) {
                XDestroyWindow(m_display, *m_window);
            }
        }
        XSync(m_display, False);
        XCloseDisplay(m_display);
    }
    if (m_pid > 0) {
        kill(m_pid, SIGTERM);
    }
    close(m_serverfd);
    close(m_latestsocket);
    delete (m_modmask);
}

/**
 * Calls RegisterWindow Logic in a new thread.
 */
void VimEmulator::RegisterWindow() {
    std::thread(&VimEmulator::RegisterWindowThread, this).detach();
}

/**
 * Calls ResizeWindow Logic in a new thread.
 *
 * @param w Width
 * @param h Height
 */
void VimEmulator::ResizeWindow(int w, int h) {
    std::thread(&VimEmulator::ResizeWindowThread, this, w, h).detach();
}

/**
 * Calls QueueFrame Logic in a new thread if the previous frame hasn't been
 * used.
 */
void VimEmulator::QueueFrame() {
    if (m_frameReady) {
        return;
    }
    std::thread(&VimEmulator::QueueFrameThread, this).detach();
}

/**
 * Translates an SDL_Keycode and sends it to the terminal.
 *
 * @param key An SDL_Keycode
 */
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

/**
 * Sets the modmask using an SDL_Keymod.
 *
 * @param mod An SDL_Keymod
 */
void VimEmulator::SetSDLMod(SDL_Keymod mod) { *m_modmask = mod; }

/** IRender **/

/**
 * @brief Render cycle for a new frame.
 *
 * If a frame is not ready it will request a new frame from the terminal. It
 * will render the previous frame until a new one is found.
 *
 * @param renderer SDL_Renderer context
 */
void VimEmulator::Render() {
    if (m_frameReady) {
        SDL_Surface *surface = this->GetFrameAsSurface();
        SetTexture(surface->pixels, surface->w, surface->h);
        this->QueueFrame();
    }
    if (glIsTexture(m_texture)) {
        TexturedRect2D::Render();
    }
};

/** Private Methods **/

/**
 * Returns the current surface and marks the current frame as not ready.
 *
 * @return SDL_Surface
 */
SDL_Surface *VimEmulator::GetFrameAsSurface() {
    m_frameReady = false;
    return m_surface;
}

/**
 * Finds and returns a window on the current system. Returns nullptr if not
 * found.
 *
 * @param window X11 Window to find.
 *
 * @return Found Window or nullptr
 */
Window *VimEmulator::findWindowByName(Window window) {
    Window parent;
    Window *children;
    unsigned int numChildren;
    char *windowName;

    if (XQueryTree(m_display, window, &window, &parent, &children,
                   &numChildren) == 0) {
        return nullptr;
    }
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
    return nullptr;
}

/**
 * Assigns the terminal window and window image to variables.
 */
void VimEmulator::RegisterWindowThread() {
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
            XSetInputFocus(m_display, m_rootWindow, RevertToPointerRoot,
                           CurrentTime);
            XMoveWindow(m_display, *m_window, -attributes.width * 2 - 1,
                        -attributes.height * 2 - 1);
            XSelectInput(m_display, *m_window, StructureNotifyMask);

            m_xImage = XGetImage(m_display, *m_window, 0, 0, m_width, m_height,
                                 attributes.depth, ZPixmap);
        }
    }
}

/**
 * Performs a resize on the terminal window and re-registers it.
 *
 * @param w Width
 * @param h Height
 */
void VimEmulator::ResizeWindowThread(int w, int h) {
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
                XResizeWindow(m_display, *m_window, m_width, m_height);
            }
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_MS));
    }
    this->RegisterWindow();
}

/**
 * Looks for an X11 ConfigureNotify event matching the given width and height.
 *
 * @param w Width
 * @param h Height
 * @param event X11 Event
 *
 * @return True if match is found
 */
bool VimEmulator::MatchResizeEvent(int w, int h, XEvent *event) {
    XNextEvent(m_display, event);
    if (event->type == ConfigureNotify) {
        XConfigureEvent xce = event->xconfigure;
        return xce.width == w && xce.height == h;
    }
    return false;
}

/**
 * Requests the current terminal frame as an image.
 */
void VimEmulator::QueueFrameThread() {
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
        XGetSubImage(m_display, *m_window, 0, 0, m_width, m_height, AllPlanes,
                     ZPixmap, m_xImage, 0, 0);
        m_surface = SDL_CreateRGBSurfaceFrom(
            (void *)m_xImage->data, m_xImage->width, m_xImage->height,
            m_xImage->depth, m_xImage->bytes_per_line, R_MASK, G_MASK, B_MASK,
            A_MASK);
        if (m_surface == nullptr) {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        m_frameReady = true;
    }
}

void VimEmulator::InitializeTCPLayer() {
    int opt = 1;

    if ((m_serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(m_serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) != 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(TCP_PORT);

    if (bind(m_serverfd, (struct sockaddr *)&m_address, sizeof(m_address)) <
        0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(m_serverfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << TCP_PORT << std::endl;
    std::thread(&VimEmulator::InitializeTCPLayerThread, this).detach();
}

void VimEmulator::InitializeTCPLayerThread() {
    {
        std::lock_guard<std::mutex> lock(m_tcpMutex);
        int addrlen = sizeof(m_address);
        while (m_latestsocket == -1) {
            if ((m_latestsocket =
                     accept(m_serverfd, (struct sockaddr *)&m_address,
                            (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                break;
            }

            std::cout << "Connection accepted" << std::endl;
        }
    }
}

void VimEmulator::SendToBuffer(std::string message) {
    if (!m_recievingBuffer){
        return;
    }
    std::thread thread(&VimEmulator::SendToBufferThread, this, message);
    SetThreadPriority(thread, 5);
    thread.detach();
}

void VimEmulator::SendToBufferThread(std::string message) {
    if (message.length() < 1) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(m_tcpMutex);
        bool callback = false;
        char recvBuffer[256] = {0};
        send(m_latestsocket, message.c_str(), message.length(), 0);
        while (!callback) {
            if (recv(m_latestsocket, &recvBuffer, 256, 0) == -1) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(REFRESH_MS));
                std::cout << "WAITING" << std::endl;
            } else {
                callback = strcmp(recvBuffer, "RECV") == 0;
            }
        }
    }
}

void VimEmulator::StartBufferReciever() {
    m_recievingBuffer = true;
    std::thread thread(&VimEmulator::BufferRecieverThread, this);
    SetThreadPriority(thread, 10);
    thread.detach();
}

void VimEmulator::BufferRecieverThread() {
    while (m_recievingBuffer) {
        if (m_requestReady) {
            continue;
        }
        {
            std::lock_guard<std::mutex> lock(m_tcpMutex);
            m_requestReady = false;
            char recvBuffer[256] = {0};
            if (recv(m_latestsocket, &recvBuffer, 256, MSG_DONTWAIT) != -1) {
                if (strcmp(recvBuffer, "RECV") == 0) {
                    std::cerr << "Recieve callback transmitted, sync has failed"
                              << std::endl;
                    exit(EXIT_FAILURE);
                }
                m_requestResult = std::string(recvBuffer, 256);
                m_requestReady = true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_MS));
    }
}

bool VimEmulator::IsRequestReady() { return m_requestReady; }

std::string VimEmulator::GetRequest() {
    m_requestReady = false;
    return m_requestResult;
}

void VimEmulator::StopBufferReciever() { m_recievingBuffer = false; }

void VimEmulator::SetThreadPriority(std::thread& thread, int priority) {
	sched_param sch_params;
	sch_params.sched_priority = priority;
	pthread_setschedparam(thread.native_handle(), SCHED_FIFO, &sch_params);
}

