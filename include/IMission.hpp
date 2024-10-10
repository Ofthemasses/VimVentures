#ifndef IMISSION_HPP
#define IMISSION_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>

class IMission {
  public:
    virtual void Run() = 0;
    virtual ~IMission() = default;
};

#endif
