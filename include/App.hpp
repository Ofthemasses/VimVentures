#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>
#include "VimEmulator.hpp"
#include "IState.hpp"
#include "IRender.hpp"

class App {
    public:
        App(Uint32 ssFlags, int x, int y, int w, int h);

        ~App();

        SDL_Renderer* GetRenderer();

        /** State Management **/
        void SetState(IState* state);

        void Run();

        void Stop();

        void Render();

        // TODO Create ?Doubly? Linked List for rendering
        void AddRenderable(IRender* renderable);

        /** Getters **/
        double GetFPS();

        int GetWidth();

        int GetHeight();

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        // For now we only need one renderable, should be updated to a vector in future
        IRender* m_renderable;

        /** Program Details **/
        int m_width;
        int m_height;
        bool m_running;

        /** State **/
        IState* m_state;

        /** FPS **/
        Uint32 m_startTick;
        Uint32 m_endTick;
};

#endif
