#include "App.hpp"

#include "constants.hpp"

#include <iostream>

App::App(Uint32 ssFlags, int x, int y, int w, int h){
    m_width = w;
    m_height = h;

    if(SDL_Init(ssFlags) < 0){
        std::cout << "SDL could not initalize: "
            << SDL_GetError();
    }

    m_window = SDL_CreateWindow(APP_TITLE, x, y, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL); // TODO Look into Vulkan
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

App::~App(){
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

SDL_Renderer* App::GetRenderer(){
    return m_renderer;
}

void App::Run(){
    if(m_running){
        std::cout << "App is already running" << std::endl;
    }
    m_running = true;

    while(m_running){
        // TODO move this
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                this->Stop();
            }
        }
        // TODO move rendering, this is test code
        SDL_SetRenderDrawColor(m_renderer, 131, 50, 168, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_renderer);
        SDL_Rect background;
        background.x = 0;
        background.y = 0;
        background.w = 1280;
        background.h = 720;
        SDL_RenderDrawRect(m_renderer, &background);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
        SDL_RenderCopy(m_renderer, texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
        SDL_DestroyTexture(texture);
    }
}

void App::Stop(){
    m_running = false;
}

void App::AddSurface(SDL_Surface *surface){
    m_surface = surface;
}
