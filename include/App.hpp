#ifndef APP_HPP
#define APP_HPP

#include "IRender.hpp"
#include "IState.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glad/glad.h>
#include <memory>
#include <vector>

class App {
  public:
    App(Uint32 ssFlags, int x, int y, int w, int h);

    ~App();

    SDL_Renderer *GetRenderer();

    /** State Management **/
    void SetState(IState *state);

    void Run();

    void Stop();

    void Render();

    void AddRenderable(std::shared_ptr<IRender> const &renderable);

    void ClearRenderables();

    /** Getters **/
    [[nodiscard]] double GetFPS() const;

    [[nodiscard]] int GetWidth() const;

    [[nodiscard]] int GetHeight() const;

  private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    std::vector<std::shared_ptr<IRender>> m_renderables;

    /** Program Details **/
    int m_width;
    int m_height;
    bool m_running;

    /** State **/
    IState *m_state;

    /** FPS **/
    static constexpr double SECOND_MS = 1000.0;
    Uint32 m_startTick;
    Uint32 m_endTick;

    // Shaders
    static constexpr int APP_GL_DEPTH_SIZE = 24;
    static void debugMessage(GLenum source, GLenum type, GLuint debug_id,
                             GLenum severity, GLsizei length,
                             const GLchar *message, const void *userParam);
    static void CreateGraphicsPipeline();

    // Fonts
    static constexpr int FONT_RENDER_SIZE = 128;
    static void GenerateFonts();

    // Draw
    void PreDraw() const;
};

#endif
