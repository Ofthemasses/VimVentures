#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>

class App {
    public:
        App(Uint32 ssFlags, int x, int y, int w, int h);

        ~App();

        SDL_Renderer* GetRenderer();

        void Run();

        void Stop();

    private:
        // SDL Vars
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        // Program Details
        int m_width;
        int m_height;
        bool m_running;
};

#endif
