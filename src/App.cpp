#include "App.hpp"

#include "constants.hpp"
#include <iostream>
#include <unistd.h>

/**
 * VimVentures Game.
 *
 * @param ssFlags Subsystem Flags
 * @param x Window X Position
 * @param y Window Y Position
 * @param w Window Width
 * @param h Window Height
 */
App::App(Uint32 ssFlags, int x, int y, int w, int h) {
    m_width = w;
    m_height = h;

    if (SDL_Init(ssFlags) < 0) {
        std::cerr << "SDL could not initalize: " << SDL_GetError();
    }

    m_window = SDL_CreateWindow(APP_TITLE, x, y, m_width, m_height,
                                SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_OPENGL); // TODO Look into Vulkan
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

/**
 * Destroy Game Window and Instance.
 */
App::~App() {
    delete (m_state);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

/**
 * @return SDL_Renderer context
 */
SDL_Renderer *App::GetRenderer() { return m_renderer; }

/**
 * Sets the game state.
 *
 * @param Game state
 */
void App::SetState(IState *state) { m_state = state; }

/**
 * Begin game cycle.
 */
void App::Run() {
    if (m_running) {
        std::cerr << "App is already running" << std::endl;
    }
    m_running = true;

    while (m_running) {
        // TODO move this
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            m_state->SendEvent(event);
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    std::cout << "FPS: " << this->GetFPS() << std::endl;
                }
            }
        }
        m_startTick = SDL_GetTicks();
        m_state->Run();
        m_endTick = SDL_GetTicks();
    }
}

/**
 * Stop Game Cycle.
 */
void App::Stop() { m_running = false; }

/**
 * Render cycle, calls all renderables.
 */
void App::Render() {
    SDL_RenderClear(m_renderer);
    // TODO ?Doubly? Linked List of Renderables
    m_renderable->Render(m_renderer);
    SDL_RenderPresent(m_renderer);
}

/**
 * Add a new renderable.
 *
 * @param Renderable
 */
void App::AddRenderable(IRender *renderable) { m_renderable = renderable; }

/**
 * @return Game FPS
 */
double App::GetFPS() const { return SECOND_MS / (m_endTick - m_startTick); }

/**
 * @return Window Width
 */
int App::GetWidth() const { return m_width; }

/**
 * @return Window Height
 */
int App::GetHeight() const { return m_height; }
