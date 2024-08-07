#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <optional>
#include <functional>
#include <glad/glad.h>
#include <Error.hpp>

class GraphicsController{
    public:
        static std::optional<Error> CheckGLObjectStatus(GLuint object, GLenum parameter,
                const std::function<void(GLuint, GLenum, GLint*)>& getiv,
                const std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)>& getInfoLog
                );

};
#endif
