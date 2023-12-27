#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP

#include <string>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>

#include "IRender.hpp"
#include "Rect2D.hpp"

class VimEmulator : public IRender, public Rect2D{
    public:
        VimEmulator(std::string terminal, std::string nArg);
        ~VimEmulator();
        void RegisterWindow();
        void ResizeWindow(int w, int h);

        void QueueFrame();
        bool FrameReady();
        XImage* GetFrame();
        SDL_Surface* GetFrameAsSurface();

        // XKeyBoard
        void SendSDLKey(SDL_Keycode key);
        void SetSDLMod(SDL_Keymod mod);

        // IRender
        void Render(SDL_Renderer* renderer);
    private:
        Display* m_display;
        int m_screen;
        Window m_rootWindow;
        Window* m_window;
        std::string m_windowName;
        int m_width;
        int m_height;

        // Frames
        XImage* m_xImage;
        SDL_Surface* m_surface;
        bool m_frameReady;

        // XKeyboard
        unsigned int* m_modmask;

        // Render Data
        SDL_Texture* m_texture;

        Window* findWindowByName(Window window);
};

#endif
