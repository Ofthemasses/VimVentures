#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP

#include <string>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>

#include "IRender.hpp"
#include "Rect2D.hpp"

class VimEmulator : public IRender, public Rect2D {
    public:
        VimEmulator(std::string terminal, std::string nArg);
        ~VimEmulator() override;
        void RegisterWindow();
        void ResizeWindow(int w, int h);
        void QueueFrame();

        /** X11 Keyboard **/
        void SendSDLKey(SDL_Keycode key);
        void SetSDLMod(SDL_Keymod mod);

        /** IRender **/
        void Render(SDL_Renderer* renderer) override;
    private:
        Display* m_display;
        int m_screen;
        Window m_rootWindow;
        Window* m_window;
        std::string m_windowName;
        int m_width;
        int m_height;

        /** Frames **/
        static constexpr Uint32 R_MASK = 0XFF0000;
        static constexpr Uint32 G_MASK = 0X00FF00;
        static constexpr Uint32 B_MASK = 0X0000FF;
        static constexpr Uint32 A_MASK = 0;
        XImage* m_xImage;
        SDL_Surface* m_surface;
        bool m_frameReady;

        /** XKeyboard **/
        unsigned int* m_modmask;

        /** Render Data **/
        SDL_Texture* m_texture;

        /** Private Methods **/
        SDL_Surface* GetFrameAsSurface();
        Window* findWindowByName(Window window);
};

#endif
