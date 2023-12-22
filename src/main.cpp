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
    app->SetVimEmulator(ve);
    // This needs to be fixed, possibly fork and wait
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ve->RegisterWindow();
    app->AddSurface(ve->GetFrameAsSurface());
    int fd[2];

    if (pipe(fd) == -1){
        std::cout << "Error generating pipe" << std::endl;
        return -1;
    }

    // Replace this with an interface or something similar, this shouldn't be done in
    // main before PR is complete
    //pid_t pid = fork();
    //if (pid == 0){ 
    //    close(fd[0]);

    //   while (true) {
    //        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    //        SDL_Surface* surface = ve->GetFrameAsSurface();
    //        write(fd[1], surface, sizeof(*surface));
    //    }

    //    close(fd[1]);
    //    return -1;
    //}
    //close(fd[1]);
    //app->SetReadSurface(fd[0]);
    app->Run();
}
