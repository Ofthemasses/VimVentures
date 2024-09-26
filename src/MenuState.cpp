#include "MenuState.hpp"
#include "GraphicsController.hpp"
#include "MissionState.hpp"
#include <SDL2/SDL_ttf.h>
#include <TexturedRect2D.hpp>
#include <memory>

/**
 * Mission Level State.
 */
MenuState::MenuState(App &app) : app(app) {
    // Clear Renderables
    app.ClearRenderables();

    // Title
    SDL_Surface *titleText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(), "VimVentures",
        TITLE_COLOR);
    float titleX = 0 - titleText->w * TITLE_W_SCALE / 2 + TITLE_X_OFFSET;
    float titleY = 0 - titleText->h * TITLE_H_SCALE / 2 + TITLE_Y_OFFSET;
    float titleWidth = titleText->w * TITLE_W_SCALE;
    float titleHeight = titleText->h * TITLE_H_SCALE;
    m_titleText = std::make_shared<TexturedRect2D>(titleX, titleY, titleWidth,
                                                   titleHeight);
    m_titleText->UpdateVertexData();
    m_titleText->EnableTextureBlend();
    m_titleText->SetTextureFormat(GL_RGBA);
    m_titleText->SetTexture(titleText->pixels, titleText->w, titleText->h);
    app.AddRenderable(m_titleText);
}

/**
 * Destroy the terminal emulator on deletion.
 */
MenuState::~MenuState() = default;

/**
 * Runs the state loop.
 */
void MenuState::Run() {
    // Render Sequence
    app.Render();
}

/**
 * Handles SDL Input.
 */
void MenuState::SendEvent(SDL_Event &event) {
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
