#include "IntroState.hpp"
#include <SDL2/SDL_image.h>
#include <MissionState.hpp>

/**
 * Mission Level State.
 */
IntroState::IntroState(App &app) : app(app) {
    // Clear Renderables
    app.ClearRenderables();

    SDL_Surface* slides = IMG_Load("./assets/images/CutScene1.png");

    m_slideshow = std::make_shared<TexturedRect2D>(-1.0/2, -1.0/2, 1.0, 1.0);
    m_slideshow->UpdateVertexData();
    m_slideshow->SetTextureFormat(GL_RGB);
    m_slideshow->SetTexture(slides->pixels, slides->w, slides->h);

    app.AddRenderable(m_slideshow);
}

/**
 * Destroy the terminal emulator on deletion.
 */
IntroState::~IntroState() = default;

/**
 * Runs the state loop.
 */
void IntroState::Run() {
    // Render Sequence
    app.Render();

    float currX = m_slideshow->GetX();
    if (currX <= STOP) {
        m_slideshow->SetPosition(currX + app.DeltaTime() / 1000.0, m_slideshow->GetY());
        m_slideshow->UpdateGL();
    }
}

/**
 * Handles SDL Input.
 */
void IntroState::SendEvent(SDL_Event &event) {
    if (event.type == SDL_QUIT) {
        // Stopping should be handled in states so we can give
        // warnings.
        app.Stop();
    } else if (event.type == SDL_KEYDOWN) {
        // The selection key should be L. Similar to vim file manager.
        switch (event.key.keysym.sym) {
        case SDLK_l:
            app.SetState(new MissionState(app));
            break;
        default:
            break;
        }
    }
}
