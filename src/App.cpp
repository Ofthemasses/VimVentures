#include "App.hpp"

#include "Error.hpp"
#include "GraphicsController.hpp"
#include "ShaderProgramBuilder.hpp"
#include "constants.hpp"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <variant>

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, APP_GL_DEPTH_SIZE);

    m_window = SDL_CreateWindow(APP_TITLE, x, y, m_width, m_height,
                                SDL_WINDOW_OPENGL); // TODO Look into Vulkan

    GraphicsController::initGL(m_window);
    GraphicsController::enableDebug();

    CreateGraphicsPipeline();
}

/**
 * Builds and loads the canode shader into the shader map.
 *
 * @todo Incorporate other shaders, perhaps a constexpr list
 */
void App::CreateGraphicsPipeline() {

    ShaderProgramBuilder shaderBuilder;

    shaderBuilder.LoadShaderFile(GL_VERTEX_SHADER, "./shaders/basic.vert")
        .LoadShaderFile(GL_FRAGMENT_SHADER, "./shaders/basic.frag");

    std::variant<Error, std::unique_ptr<ShaderProgram>> shaderProgramResult =
        shaderBuilder.GenerateShaderProgram();

    if (std::holds_alternative<Error>(shaderProgramResult)) {
        std::cout << std::get<Error>(shaderProgramResult).toString()
                  << std::endl;
    } else {
        GraphicsController::s_shaderPrograms.try_emplace(
            std::string("sp_cathode"),
            std::move(
                std::get<std::unique_ptr<ShaderProgram>>(shaderProgramResult)));
    }
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
void App::SetState(IState *state) {
    delete (m_state);
    m_state = state;
}

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

void App::PreDraw() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, m_width, m_height);
    glClearColor(1.0F, 1.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
}
/**
 * Render cycle, calls all renderables.
 */
void App::Render() {
    PreDraw();
    // SDL_RenderClear(m_renderer);
    // TODO ?Doubly? Linked List of Renderables
    if (m_renderable != nullptr) {
        m_renderable->Render();
    }
    glUseProgram(0);
    // SDL_RenderPresent(m_renderer);
    SDL_GL_SwapWindow(m_window);
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
