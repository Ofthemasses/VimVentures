#include "MissionState.hpp"

/**
 * Mission Level State.
 */
MissionState::MissionState(App &app) : app(app) {
    m_vimEmulator = new VimEmulator("alacritty", "--title");

    m_vimEmulator->RegisterWindow();

    int terminalW = (int)(app.GetWidth() * WIDTH_RATIO);
    int terminalH = (int)(app.GetHeight() * WIDTH_RATIO);
    m_vimEmulator->ResizeWindow(terminalW, terminalH);
    m_vimEmulator->SetSize(terminalW, terminalH);

    int posX = (app.GetWidth() - terminalW) / 2;
    int posY = (app.GetHeight() - terminalH) / 2;
    m_vimEmulator->SetPosition(posX, posY);

    m_vimEmulator->QueueFrame();

    app.AddRenderable(m_vimEmulator);
}

/**
 * Destroy the terminal emulator on deletion.
 */
MissionState::~MissionState() { delete (m_vimEmulator); }

/**
 * Runs the state loop.
 */
void MissionState::Run() {
    // Render Sequence
    SDL_SetRenderDrawColor(app.GetRenderer(), DRAW_R, DRAW_G, DRAW_B,
                           SDL_ALPHA_OPAQUE);
    app.Render();
}

/**
 * Handles SDL Input.
 */
void MissionState::SendEvent(SDL_Event &event) {
    if (event.type == SDL_QUIT) {
        // Stopping should be handled in states so we can give
        // warnings.
        app.Stop();
    } else if (event.type == SDL_KEYDOWN) {
        m_vimEmulator->SetSDLMod((SDL_Keymod)event.key.keysym.mod);
        m_vimEmulator->SendSDLKey(event.key.keysym.sym);
    }
}
