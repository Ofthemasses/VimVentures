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

    VimEmulator* ve = new VimEmulator("alacritty", "--title");
    app->SetVimEmulator(ve);
    // This needs to be fixed, possibly fork and wait
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ve->RegisterWindow();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->AddSurface(ve->GetFrameAsSurface());
    int fd[2];

    if (pipe(fd) == -1){
        std::cout << "Error generating pipe" << std::endl;
        return -1;
    }

    app->Run();
}
