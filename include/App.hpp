#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>
#include "VimEmulator.hpp"

class App {
    public:
        App(Uint32 ssFlags, int x, int y, int w, int h);

        ~App();

        SDL_Renderer* GetRenderer();

        void Run();

        void Stop();

        void AddSurface(SDL_Surface *surface);

        void SetReadSurface(int readSurface);

        void SetVimEmulator(VimEmulator* ve);

    private:
        // SDL Vars
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        // For now we only need one surface, should be updated to an array in future
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;
        int m_readSurface;
        // Program Details
        int m_width;
        int m_height;
        bool m_running;

        // Test
        VimEmulator* m_ve;
};

#endif
