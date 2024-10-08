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

    m_helpMonitorImage = IMG_Load("./assets/images/HelperTerminal.png");
    
    m_helpMonitorSurface = SDL_CreateRGBSurface(0, m_helpMonitorImage->w, 
            m_helpMonitorImage->h, 
            m_helpMonitorImage->format->BitsPerPixel,
            m_helpMonitorImage->format->Rmask,
            m_helpMonitorImage->format->Gmask,
            m_helpMonitorImage->format->Bmask,
            m_helpMonitorImage->format->Amask);

    SDL_BlitSurface(m_helpMonitorImage, nullptr, m_helpMonitorSurface, nullptr);

    m_helpMonitor = std::make_shared<TexturedRect2D>(HELPER_X_POSITION, -WIDTH_RATIO / 2, HELPER_WIDTH, WIDTH_RATIO);
    m_helpMonitor->SetTextureFormat(GL_RGB);
    m_helpMonitor->EnableTextureBlend();
    m_helpMonitor->UpdateVertexData();
    m_helpMonitor->SetTexture(m_helpMonitorSurface->pixels, m_helpMonitorSurface->w, m_helpMonitorSurface->h);
    m_helpMonitor->SetShaderProgram("sp_cathode");
    app.AddRenderable(m_helpMonitor);

    m_currentMission = std::make_unique<TutorialMission>(app, *this, m_vimEmulator);
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

// TODO OpenGL Blitting may work nicer due to floating point calculations
void MissionState::UpdateHelperMonitor(std::string text){
    // Clear text
    SDL_BlitSurface(m_helpMonitorImage, nullptr, m_helpMonitorSurface, nullptr);

    const int WRAP = 128*8;
    SDL_Surface *helpText = TTF_RenderText_Blended_Wrapped(
        GraphicsController::s_fonts.at("ttf_terminus").get(), text.c_str(), 
        {255, 255, 255}, WRAP);

    const float WIDTH = ((float) m_helpMonitorImage->w - HELPER_TEXT_PADDING*2.0) * (float) helpText->w / WRAP;
    const float HEIGHT = (WIDTH * helpText->h) / ((float) helpText->w);
    auto *helpTextRect = new SDL_Rect{HELPER_TEXT_PADDING, HELPER_TEXT_PADDING, (int) WIDTH, (int) HEIGHT};

    SDL_BlitScaled(helpText, nullptr, m_helpMonitorSurface, helpTextRect);
    m_helpMonitor->SetTexture(m_helpMonitorSurface->pixels, m_helpMonitorSurface->w, m_helpMonitorSurface->h);
    m_helpMonitor->UpdateGL();
}
