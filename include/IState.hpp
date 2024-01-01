#ifndef ISTATE_HPP
#define ISTATE_HPP

#include <SDL2/SDL.h>

class IState {
public:
  virtual void Run() = 0;
  virtual void SendEvent(SDL_Event &event) = 0;
  virtual ~IState() = default;
};

#endif
