#include "MenuState.hpp"
#include "MissionState.hpp"

/**
 * Mission Level State.
 */
MenuState::MenuState(App &app) : app(app) {}

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
