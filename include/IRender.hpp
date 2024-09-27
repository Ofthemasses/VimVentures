#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>

class IRender {
  protected:
    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    GLuint m_indexBufferObject;
    std::string m_shaderProgram;
    virtual void GenBindBufferGL() = 0;

  public:
    virtual void Render() = 0;
    virtual void UpdateGL() = 0;
};

#endif
