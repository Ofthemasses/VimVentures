#include "MissionState.hpp"
#include "MenuState.hpp"
#include <TutorialMission.hpp>
#include <memory>
#include <SDL2/SDL_image.h>

/**
 * Mission Level State.
 */
MissionState::MissionState(App &app) : app(app) {
    // Clear Renderables
    app.ClearRenderables();

    m_vimEmulator = std::make_shared<VimEmulator>("alacritty", "--title");

    m_vimEmulator->SetShaderProgram("sp_cathode");
    m_vimEmulator->RegisterWindow();

    int terminalW = (int)(app.GetWidth() * WIDTH_RATIO);
    int terminalH = (int)(app.GetHeight() * WIDTH_RATIO);
    m_vimEmulator->ResizeWindow(terminalW, terminalH);
    m_vimEmulator->SetSize(WIDTH_RATIO, WIDTH_RATIO);

    m_vimEmulator->SetPosition(-WIDTH_RATIO / 2, -WIDTH_RATIO / 2);

    m_vimEmulator->QueueFrame();


    SDL_Surface* frameImage = IMG_Load("./assets/images/Terminal.png");
    auto frame = std::make_shared<TexturedRect2D>(-0.5, -0.5, 1.0, 1.0);
    frame->SetTextureFormat(GL_RGBA);
    frame->UpdateVertexData();
    frame->SetTexture(frameImage->pixels, frameImage->w,
                            frameImage->h);
    app.AddRenderable(frame);
    // Start Tutorial
    m_currentMission = std::make_unique<TutorialMission>(app, m_vimEmulator);
    app.AddRenderable(m_vimEmulator);
}

/**
 * Destroy the terminal emulator on deletion.
 */
MissionState::~MissionState() = default;

/**
 * Runs the state loop.
 */
void MissionState::Run() {
    // Render Sequence
    app.Render();
    m_currentMission->Run();
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
        if (event.key.keysym.sym == SDLK_F12) {
            app.AddRenderable(nullptr);
            app.SetState(new MenuState(app));
            return;
        }
        m_vimEmulator->SetSDLMod((SDL_Keymod)event.key.keysym.mod);
        m_vimEmulator->SendSDLKey(event.key.keysym.sym);
    }
}
