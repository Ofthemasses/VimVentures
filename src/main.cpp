#include "App.hpp"
#include "MenuState.hpp"
#include <SDL2/SDL.h> // Does SDL inclusion need to be in main.cpp?
#include <glad/glad.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    App *app = new App(SDL_INIT_VIDEO, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    app->SetState(new MenuState(*app));

    app->Run();

    delete (app);

    return 0;
}
