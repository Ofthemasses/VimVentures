#ifndef GRAPHICSCONTROLLER_HPP
#define GRAPHICSCONTROLLER_HPP

#include <Error.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ShaderProgram.hpp>
#include <functional>
#include <glad/glad.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

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
    static std::unordered_map<std::string, std::unique_ptr<ShaderProgram>>
        s_shaderPrograms;
    static std::unordered_map<
        std::string, std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>>>
        s_fonts;

  private:
    static SDL_GLContext s_glContext;
};
#endif
