#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <Error.hpp>
#include <SDL2/SDL.h>
#include <functional>
#include <glad/glad.h>
#include <optional>

class GraphicsController {
  public:
    static void debugMessage(GLenum source, GLenum type, GLuint debug_id,
                             GLenum severity, GLsizei length,
                             const GLchar *message, const void *userParam);
    static void enableDebug();
    static std::optional<Error> initGL(SDL_Window *sdlWindow);
    static std::optional<Error> CheckGLObjectStatus(
        GLuint object, GLenum parameter,
        const std::function<void(GLuint, GLenum, GLint *)> &getiv,
        const std::function<void(GLuint, GLsizei, GLsizei *, GLchar *)>
            &getInfoLog);

  private:
    static SDL_GLContext s_glContext;
};
#endif
