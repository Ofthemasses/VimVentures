#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>

class IRender {
  protected:
    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    GLuint m_indexBufferObject;
    virtual void GenBindBufferGL() = 0;
  public:
    virtual void Render() = 0;
    virtual void UpdateGL() = 0;
};

#endif
