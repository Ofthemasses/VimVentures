#include "App.hpp"

#include "constants.hpp"
#include <unistd.h>
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

void App::SetState(IState* state){
    m_state = state;
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
            m_state->SendEvent(event);
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_SPACE){
                    std::cout << "FPS: " << this->GetFPS() << std::endl;
                }
            }
        }
        m_startTick = SDL_GetTicks();
        m_state->Run();
        m_endTick = SDL_GetTicks();
    }
}

void App::Stop(){
    m_running = false;
}

void App::Render(){
    SDL_RenderClear(m_renderer);
    // TODO ?Doubly? Linked List of Renderables
    m_renderable->Render(m_renderer);
    SDL_RenderPresent(m_renderer);
}

void App::AddRenderable(IRender* renderable){
    m_renderable = renderable;
}

double App::GetFPS(){
    return 1000.0 / (m_endTick - m_startTick);
}

int App::GetWidth(){
    return m_width;
}

int App::GetHeight(){
    return m_height;
}
