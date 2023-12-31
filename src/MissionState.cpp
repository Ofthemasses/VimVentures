#include "MissionState.hpp"
#include <thread>
#include <chrono>

MissionState::MissionState(App& app) : app(app){
    m_vimEmulator = new VimEmulator("alacritty", "--title");

    // This needs to be fixed, possibly fork and wait
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_vimEmulator->RegisterWindow();

    int terminalW = (int) (app.GetWidth() * WIDTH_RATIO);
    int terminalH = (int) (app.GetHeight() * WIDTH_RATIO);
    m_vimEmulator->ResizeWindow(terminalW ,terminalH);
    m_vimEmulator->SetSize(terminalW, terminalH);

    int posX = (app.GetWidth() - terminalW) / 2;
    int posY = (app.GetHeight() - terminalH) / 2;
    m_vimEmulator->SetPosition(posX, posY);

    m_vimEmulator->QueueFrame();

    app.AddRenderable(m_vimEmulator);
}

MissionState::~MissionState(){
    delete(m_vimEmulator);
}

void MissionState::Run(){
    // Render Sequence
    SDL_SetRenderDrawColor(app.GetRenderer(), DRAW_R, DRAW_G, DRAW_B, SDL_ALPHA_OPAQUE);
    app.Render();
}

void MissionState::SendEvent(SDL_Event& event){
    if(event.type == SDL_QUIT){
        // Stopping should be handled in states so we can give
        // warnings.
        app.Stop();
    } else if (event.type == SDL_KEYDOWN){
        m_vimEmulator->SetSDLMod((SDL_Keymod)event.key.keysym.mod);
        m_vimEmulator->SendSDLKey(event.key.keysym.sym);
    }
}


