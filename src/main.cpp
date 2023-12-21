#include <SDL2/SDL.h> // Does SDL inclusion need to be in main.cpp?

#include "App.hpp"

int main(int argc, char* argv[]){
    App* app = new App(SDL_INIT_VIDEO, 0, 0, 1280, 720);
    app->Run();
}
