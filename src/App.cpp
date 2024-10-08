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
    if (TTF_Init() == 1) {
        std::cout << "Could not initialize SDL2 ttf, error: " << TTF_GetError()
                  << std::endl;
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
    GenerateFonts();
}

/**
 * Builds and loads the canode shader into the shader map.
 *
 * @todo Incorporate other shaders, perhaps a constexpr list
 */
void App::CreateGraphicsPipeline() {

    ShaderProgramBuilder shaderBuilder;

    shaderBuilder.LoadShaderFile(GL_VERTEX_SHADER, "./shaders/cathode.vert")
        .LoadShaderFile(GL_FRAGMENT_SHADER, "./shaders/cathode.frag");

    std::variant<Error, std::unique_ptr<ShaderProgram>> cathodeShaderResult =
        shaderBuilder.GenerateShaderProgram();

    if (std::holds_alternative<Error>(cathodeShaderResult)) {
        std::cout << std::get<Error>(cathodeShaderResult).toString()
                  << std::endl;
    } else {
        GraphicsController::s_shaderPrograms.try_emplace(
            std::string("sp_cathode"),
            std::move(
                std::get<std::unique_ptr<ShaderProgram>>(cathodeShaderResult)));
    }

    shaderBuilder.LoadShaderFile(GL_VERTEX_SHADER, "./shaders/basic.vert")
        .LoadShaderFile(GL_FRAGMENT_SHADER, "./shaders/basic.frag");

    std::variant<Error, std::unique_ptr<ShaderProgram>> basicShaderResult =
        shaderBuilder.GenerateShaderProgram();

    if (std::holds_alternative<Error>(basicShaderResult)) {
        std::cout << std::get<Error>(basicShaderResult).toString() << std::endl;
    } else {
        GraphicsController::s_shaderPrograms.try_emplace(
            std::string("sp_basic"),
            std::move(
                std::get<std::unique_ptr<ShaderProgram>>(basicShaderResult)));
    }

    shaderBuilder.LoadShaderFile(GL_VERTEX_SHADER, "./shaders/gradient.vert")
        .LoadShaderFile(GL_FRAGMENT_SHADER, "./shaders/gradient.frag");

    std::variant<Error, std::unique_ptr<ShaderProgram>> gradientShaderResult =
        shaderBuilder.GenerateShaderProgram();

    if (std::holds_alternative<Error>(gradientShaderResult)) {
        std::cout << std::get<Error>(gradientShaderResult).toString() << std::endl;
    } else {
        GraphicsController::s_shaderPrograms.try_emplace(
            std::string("sp_gradient"),
            std::move(
                std::get<std::unique_ptr<ShaderProgram>>(gradientShaderResult)));
    }
}

/**
 * Generate Fonts.
 *
 * @todo May be better to grab pre-serialised, though this matches with OpenGL
 * philosophy.
 */
void App::GenerateFonts() {
    TTF_Font *ttfTerminus =
        TTF_OpenFont("./assets/fonts/TerminusTTF-4.46.0.ttf", FONT_RENDER_SIZE);
    GraphicsController::s_fonts.try_emplace(
        "ttf_terminus",
        std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>>(
            ttfTerminus, [](TTF_Font *font) { TTF_CloseFont(font); }));
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
    m_deltaTime = (SDL_GetTicks() - m_startTick) / SECOND_MS;
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
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * Render cycle, calls all renderables.
 */
void App::Render() {
    PreDraw();

    for (std::shared_ptr<IRender> renderable : m_renderables) {
        renderable->Render();
    }

    glUseProgram(0);
    // SDL_RenderPresent(m_renderer);
    SDL_GL_SwapWindow(m_window);
}

/**
 * Add a new renderable to renderables.
 *
 * @param Renderable
 */
void App::AddRenderable(std::shared_ptr<IRender> const &renderable) {
    m_renderables.emplace_back(renderable);
}

/**
 * Clears renderables.
 */
void App::ClearRenderables() { m_renderables.clear(); }

/**
 * @return Game FPS
 */
double App::GetFPS() const { return SECOND_MS / (m_endTick - m_startTick); }

/**
 * @return Delta Time
 */
double App::DeltaTime() const { return m_deltaTime; }

/**
 * @return Window Width
 */
int App::GetWidth() const { return m_width; }

/**
 * @return Window Height
 */
int App::GetHeight() const { return m_height; }
