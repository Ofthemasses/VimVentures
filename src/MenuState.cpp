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

    selector_pos = 0;

    // Title
    SDL_Surface *titleText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(), "VimVentures",
        TEXT_COLOR);
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

    // Start
    SDL_Surface *startText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(), "Start",
        TEXT_COLOR);
    float startX = 0 - startText->w * MENU_W_SCALE / 2 + START_X_OFFSET;
    float startY = 0 - startText->h * MENU_H_SCALE / 2 + START_Y_OFFSET;
    float startWidth = startText->w * MENU_W_SCALE;
    float startHeight = startText->h * MENU_H_SCALE;
    m_startText = std::make_shared<TexturedRect2D>(startX, startY, startWidth,
                                                   startHeight);
    m_startText->UpdateVertexData();
    m_startText->EnableTextureBlend();
    m_startText->SetTextureFormat(GL_RGBA);
    m_startText->SetTexture(startText->pixels, startText->w, startText->h);
    app.AddRenderable(m_startText);

    // Quit
    SDL_Surface *quitText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(), "Quit",
        TEXT_COLOR);
    float quitX = 0 - quitText->w * MENU_W_SCALE / 2 + QUIT_X_OFFSET;
    float quitY = 0 - quitText->h * MENU_H_SCALE / 2 + QUIT_Y_OFFSET;
    float quitWidth = quitText->w * MENU_W_SCALE;
    float quitHeight = quitText->h * MENU_H_SCALE;
    m_quitText =
        std::make_shared<TexturedRect2D>(quitX, quitY, quitWidth, quitHeight);
    m_quitText->UpdateVertexData();
    m_quitText->EnableTextureBlend();
    m_quitText->SetTextureFormat(GL_RGBA);
    m_quitText->SetTexture(quitText->pixels, quitText->w, quitText->h);
    app.AddRenderable(m_quitText);

    // Selector
    SDL_Surface *selectorText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(), ">", TEXT_COLOR);
    // Start Selector on Start
    float selectorX = 0 - selectorText->w * MENU_W_SCALE / 2 -
                      startText->w * MENU_W_SCALE / 2 - SELECTOR_PADDING;
    float selectorY = 0 - selectorText->h * MENU_H_SCALE / 2 + START_Y_OFFSET;
    float selectorWidth = selectorText->w * MENU_W_SCALE;
    float selectorHeight = selectorText->h * MENU_H_SCALE;
    m_selector = std::make_shared<TexturedRect2D>(
        selectorX, selectorY, selectorWidth, selectorHeight);
    m_selector->UpdateVertexData();
    m_selector->EnableTextureBlend();
    m_selector->SetTextureFormat(GL_RGBA);
    m_selector->SetTexture(selectorText->pixels, selectorText->w,
                           selectorText->h);
    app.AddRenderable(m_selector);

    // Instructions
    SDL_Surface *instructionText = TTF_RenderText_Blended(
        GraphicsController::s_fonts.at("ttf_terminus").get(),
        "move: j/k select: l", TEXT_COLOR);
    float instructionX =
        0 - instructionText->w * INSTRUCTION_W_SCALE / 2 + INSTRUCTION_X_OFFSET;
    float instructionY =
        0 - instructionText->h * INSTRUCTION_H_SCALE / 2 + INSTRUCTION_Y_OFFSET;
    float instructionWidth = instructionText->w * INSTRUCTION_W_SCALE;
    float instructionHeight = instructionText->h * INSTRUCTION_H_SCALE;
    m_instructionText = std::make_shared<TexturedRect2D>(
        instructionX, instructionY, instructionWidth, instructionHeight);
    m_instructionText->UpdateVertexData();
    m_instructionText->EnableTextureBlend();
    m_instructionText->SetTextureFormat(GL_RGBA);
    m_instructionText->SetTexture(instructionText->pixels, instructionText->w,
                                  instructionText->h);
    app.AddRenderable(m_instructionText);
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
            Select();
            break;
        case SDLK_j:
            SelectorNext();
            break;
        case SDLK_k:
            SelectorPrev();
            break;
        default:
            break;
        }
    }
}

/**
 * Moves the selector to the next menu item.
 */
void MenuState::SelectorNext() {
    if (MENU_COUNT < selector_pos + 1) {
        std::cout << "TEST" << std::endl;
        return;
    }
    selector_pos++;

    // Switch incase of more menu options
    switch (selector_pos) {
    case QUIT:
        float selectorX = 0 - m_selector->GetWidth() / 2 -
                          m_quitText->GetWidth() / 2 - SELECTOR_PADDING;
        float selectorY = 0 - m_selector->GetHeight() / 2 + QUIT_Y_OFFSET;
        std::cout << "QUIT" << std::endl;
        m_selector->SetPosition(selectorX, selectorY);
        m_selector->UpdateGL();
        break;
    }
}

/**
 * Moves the selector to the previous menu item.
 */
void MenuState::SelectorPrev() {
    if (selector_pos == 0) {
        return;
    }

    selector_pos--;
    switch (selector_pos) {
    case START:
        float selectorX = 0 - m_selector->GetWidth() / 2 -
                          m_startText->GetWidth() / 2 - SELECTOR_PADDING;
        float selectorY = 0 - m_selector->GetHeight() / 2 + START_Y_OFFSET;
        std::cout << "START" << std::endl;
        m_selector->SetPosition(selectorX, selectorY);
        m_selector->UpdateGL();
        break;
    }
}

/**
 * Executes menu item logic based on the currently selected item.
 */
void MenuState::Select() {
    switch (selector_pos) {
    case START:
        app.SetState(new MissionState(app));
        break;
    case QUIT:
        app.Stop();
        break;
    }
}
