#include "IntroState.hpp"
#include <SDL2/SDL_image.h>
#include <MissionState.hpp>
#include <algorithm>

/**
 * Mission Level State.
 */
IntroState::IntroState(App &app) : app(app) {
    m_currSlide = 0;

    // Clear Renderables
    app.ClearRenderables();

    SDL_Surface* slideImage0 = IMG_Load("./assets/images/CutScene1.png");

    m_slideshow = std::make_shared<TexturedRect2D>(-1.4/2, -1.2/2, 1.2, 1.2);
    m_slideshow->UpdateVertexData();
    m_slideshow->SetTextureFormat(GL_RGB);
    m_slideshow->SetTexture(slideImage0->pixels, slideImage0->w, slideImage0->h);
    m_slides[0].slideSurface = slideImage0;
    m_slides[0].startX = -1.4/2;
    m_slides[0].stopX = -0.5;
    m_slides[0].startY = -1.2/2;
    m_slides[0].stopY = -1.2/2;
    m_slides[0].panSpeedMS = 5000.0;

    app.AddRenderable(m_slideshow);

    SDL_Surface* slideImage1 = IMG_Load("./assets/images/CutScene2.png");

    m_slides[1].slideSurface = slideImage1;
    m_slides[1].startX = -0.5;
    m_slides[1].stopX = -0.5;
    m_slides[1].startY = -1.4/2;
    m_slides[1].stopY = -0.5;
    m_slides[1].panSpeedMS = 5000.0;

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

    if (m_currSlide == SLIDE_COUNT){
        return;
    }
    Slide slide = m_slides[m_currSlide];
    float currX = m_slideshow->GetX();
    float currY = m_slideshow->GetY();

    if (currX == slide.stopX && currY == slide.stopY) {
        NextSlide();
        return;
    }

    float panXLength = slide.stopX - slide.startX;
    float panYLength = slide.stopY - slide.startY;
    float nextX = panXLength == 0 ? slide.stopY : std::min((float) (currX + (panXLength / slide.panSpeedMS / app.DeltaTime())), slide.stopX);
    float nextY = panYLength == 0 ? slide.stopY : std::min((float) (currY + (panYLength / slide.panSpeedMS / app.DeltaTime())), slide.stopY);
    m_slideshow->SetPosition(
            nextX,
            nextY);
    m_slideshow->UpdateGL();
}

void IntroState::NextSlide() {
    m_currSlide++;

    if (m_currSlide >= SLIDE_COUNT){
        return;
    }

    Slide slide = m_slides[m_currSlide];
    SDL_Surface* slideSurface = slide.slideSurface;
    m_slideshow->SetTexture(slideSurface->pixels, slideSurface->w, slideSurface->h);
    m_slideshow->SetPosition(slide.startX, slide.startY);
    m_slideshow->UpdateGL();
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
