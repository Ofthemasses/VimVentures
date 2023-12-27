#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <SDL2/SDL.h>

class IRender {
    public:
        virtual void Render(SDL_Renderer* renderer) = 0;
};

#endif
