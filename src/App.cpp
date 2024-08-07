#include "App.hpp"

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <memory>
#include <variant>
#include "glad/glad.h"
#include "constants.hpp"
#include "Error.hpp"
#include "ShaderProgramBuilder.hpp"

void App::debugMessage(GLenum source, GLenum type, GLuint debug_id,
                       GLenum severity, GLsizei length, const GLchar *message,
                       const void *userParam) {
    std::cout << severity << std::endl;
    std::string message_str(message, length);
    std::cout << message_str << '\n';
}
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

    m_glcontext = SDL_GL_CreateContext(m_window);

    if (m_glcontext == nullptr) {
        std::cerr << "OpenGL context not available: " << SDL_GetError();
    }

    // Initialize GL
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
        std::cerr << "Glad Not Initialised: " << std::endl;
        exit(1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessage, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
    CreateGraphicsPipeline();
}


void App::CreateGraphicsPipeline() {

    ShaderProgramBuilder shaderBuilder;

    shaderBuilder
        .LoadShaderFile(GL_VERTEX_SHADER, "./shaders/basic.vert")
        .LoadShaderFile(GL_FRAGMENT_SHADER, "./shaders/basic.frag");

    std::variant<Error, std::unique_ptr<ShaderProgram>> shaderProgramResult = shaderBuilder.GenerateShaderProgram();

    if (std::holds_alternative<Error>(shaderProgramResult)){
        std::cout << std::get<Error>(shaderProgramResult).toString() << std::endl;
    } else {
        m_GraphicsPipelineShaderProgram = std::move(std::get<std::unique_ptr<ShaderProgram>>(shaderProgramResult));
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

void App::PreDraw() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, m_width, m_height);
    glClearColor(1.0F, 1.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_GraphicsPipelineShaderProgram->GetProgramId());
}
/**
 * Render cycle, calls all renderables.
 */
void App::Render() {
    PreDraw();
    // SDL_RenderClear(m_renderer);
    // TODO ?Doubly? Linked List of Renderables
    m_renderable->Render(m_GraphicsPipelineShaderProgram->GetProgramId());
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
