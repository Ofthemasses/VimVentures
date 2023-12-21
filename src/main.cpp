#include <SDL2/SDL.h> // Does SDL inclusion need to be in main.cpp?

#include "App.hpp"
#include "VimEmulator.hpp"

#include "pty.h"

int main(int argc, char* argv[]){
    App* app = new App(SDL_INIT_VIDEO, 0, 0, 1280, 720);
    winsize w;
    w.ws_xpixel = 256;
    w.ws_ypixel = 256;
    VimEmulator* ve = new VimEmulator(w);
    while (!ve->Ready()){}
    ve->Print();
    app->Run();
}
