#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP

#include <string>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <mutex>

class VimEmulator{
    public:
        VimEmulator(std::string terminal, std::string nArg);
        ~VimEmulator();
        void RegisterWindow();

        void QueueFrame();
        bool FrameReady();
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

        // Frames
        XImage* m_xImage;
        SDL_Surface* m_surface;
        bool m_frameReady;
        std::mutex m_mutex;

        // XKeyboard
        unsigned int* m_modmask;

        Window* findWindowByName(Window window);
};

#endif
