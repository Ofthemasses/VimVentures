#include "VimEmulator.hpp"
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <constants.hpp>
#include <unistd.h>
#include <thread>
#include <chrono>

VimEmulator::VimEmulator(std::string terminal, std::string nArg){
    m_display = XOpenDisplay(NULL);
    m_screen = DefaultScreen(m_display);
    m_rootWindow = RootWindow(m_display, m_screen);
    m_windowName = std::string(APP_TITLE) + "Emulator";
    m_window = nullptr;

    // Run the terminal instance
    pid_t pid = fork();
    if(pid == 0){
        setenv("DISPLAY", XDisplayString(m_display), 1);
        execlp(terminal.c_str(),
                terminal.c_str(),
                nArg.c_str(), 
                m_windowName.c_str(),
                NULL);
        exit(EXIT_FAILURE);
    }
}

VimEmulator::~VimEmulator(){}

void VimEmulator::RegisterWindow(){
    m_window = this->findWindowByName(m_rootWindow);
    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, *m_window, &attributes);
    m_xImage = XGetImage(m_display,
            *m_window,
            0,
            0,
            attributes.width,
            attributes.height,
            AllPlanes,
            ZPixmap);
}

XImage* VimEmulator::GetFrame(){
    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, *m_window, &attributes);
    XGetSubImage(m_display,
            *m_window,
            0,
            0,
            attributes.width,
            attributes.height,
            AllPlanes,
            ZPixmap,
            m_xImage,
            0,
            0);
    return m_xImage;
}

SDL_Surface* VimEmulator::GetFrameAsSurface(){
    XImage* image = this->GetFrame();
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
            (void*) image->data,
            image->width,
            image->height,
            image->depth,
            image->bytes_per_line,
            (Uint32) 0XFF0000,
            (Uint32) 0X00FF00,
            (Uint32) 0X0000FF,
            0);
    if (surface == NULL){
        std::cout << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    return surface;
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
