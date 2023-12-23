#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP

#include <string>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>

class VimEmulator{
    public:
        VimEmulator(std::string terminal, std::string nArg);
        ~VimEmulator();
        void RegisterWindow();
        XImage* GetFrame();
        SDL_Surface* GetFrameAsSurface();

        // XKeyBoard
        void SendSDLKey(SDL_Keycode key);
        void SetSDLMod(SDL_Keymod mod);
    private:
        Display* m_display;
        int m_screen;
        Window m_rootWindow;
        Window* m_window;
        std::string m_windowName;
        XImage* m_xImage;

        // XKeyboard
        unsigned int* m_modmask;

        Window* findWindowByName(Window window);
};

#endif
