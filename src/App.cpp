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

void App::Run(){
    int terminalW = (int) (m_width * 2 / 3.0);
    int terminalH = (int) (m_height * 2 / 3.0);
    m_ve->ResizeWindow(terminalW ,terminalH);
    m_rect.w = terminalW;
    m_rect.h = terminalH;
    m_rect.x = (m_width - terminalW) / 2;
    m_rect.y = (m_height - terminalH) / 2;
    m_ve->QueueFrame();
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
            } else if (event.type == SDL_KEYDOWN){
                m_ve->SetSDLMod((SDL_Keymod)event.key.keysym.mod);
                m_ve->SendSDLKey(event.key.keysym.sym);
                if (event.key.keysym.sym == SDLK_SPACE){
                    std::cout << "FPS: " << this->GetFPS() << std::endl;
                }
            }
        }
        m_startTick = SDL_GetTicks();
        // TODO move rendering, this is test code
        SDL_SetRenderDrawColor(m_renderer, 131, 50, 168, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_renderer);
        if (m_ve->FrameReady()){
            SDL_DestroyTexture(m_texture);
            m_surface = m_ve->GetFrameAsSurface();
            m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
            SDL_FreeSurface(m_surface);
            m_ve->QueueFrame();
        }
        SDL_RenderCopy(m_renderer, m_texture, NULL, &m_rect);
        SDL_RenderPresent(m_renderer);
        m_endTick = SDL_GetTicks();
    }
}

void App::Stop(){
    delete(m_ve);
    m_running = false;
}

void App::AddSurface(SDL_Surface *surface){
    m_surface = surface;
}

void App::SetReadSurface(int readSurface){
   m_readSurface = readSurface;
} 

void App::SetVimEmulator(VimEmulator* ve){
    m_ve = ve;
}

double App::GetFPS(){
    return 1000.0 / (m_endTick - m_startTick);
}
