#include <SDL2/SDL.h> // Does SDL inclusion need to be in main.cpp?

#include "App.hpp"
#include "VimEmulator.hpp"

#include "pty.h"
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include <X11/Xlib.h>
#include <X11/Xutil.h>





int main(int argc, char* argv[]){
    App* app = new App(SDL_INIT_VIDEO, 0, 0, 1280, 720);
    winsize w;
    w.ws_xpixel = 256;
    w.ws_ypixel = 256;
    // VimEmulator* ve = new VimEmulator(w);
    // while(!ve->Ready()){}

    VimEmulator* ve = new VimEmulator("alacritty", "--title");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ve->RegisterWindow();
    // This needs to be fixed, possibly fork and wait
    app->AddSurface(ve->GetFrameAsSurface());

//    XMapWindow(display, window);
//    XFlush(display);

    app->Run();


}
