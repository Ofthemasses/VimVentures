#include <SDL2/SDL.h> // Does SDL inclusion need to be in main.cpp?

#include "App.hpp"
#include "App.hpp"
#include "MissionState.hpp"
#include <unistd.h>
#include <thread>
#include <chrono>





int main(int argc, char* argv[]){
    App* app = new App(SDL_INIT_VIDEO, 0, 0, 1280, 720);

    app->SetState(new MissionState(*app));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    app->Run();
}
