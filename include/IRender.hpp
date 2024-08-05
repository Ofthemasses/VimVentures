#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>

class IRender {
  public:
    virtual void Render(GLuint renderer) = 0;
};

#endif
