#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP
#include <SDL2/SDL.h>
#include <X11/Xlib.h>
#include <mutex>
#include <string>
#include <netinet/in.h>

#include "TexturedRect2D.hpp"

class VimEmulator : public TexturedRect2D {
  public:
    VimEmulator(std::string terminal, std::string nArg);
    virtual ~VimEmulator();
    void RegisterWindow();
    void ResizeWindow(int w, int h);
    void QueueFrame();

    /** TCP **/
    void InitializeTCPLayer();
    void SendToBuffer();

    /** X11 Keyboard **/
    void SendSDLKey(SDL_Keycode key);
    void SetSDLMod(SDL_Keymod mod);

    /** IRender **/
    void Render() override;

  private:
    static constexpr int REFRESH_MS = 100;
    Display *m_display;
    int m_screen;
    Window m_rootWindow;
    Window *m_window;
    std::string m_windowName;
    int m_width;
    int m_height;
    std::mutex m_mutex;
    pid_t m_pid;

    /** Frames **/
    static constexpr Uint32 R_MASK = 0XFF0000;
    static constexpr Uint32 G_MASK = 0X00FF00;
    static constexpr Uint32 B_MASK = 0X0000FF;
    static constexpr Uint32 A_MASK = 0;
    XImage *m_xImage;
    SDL_Surface *m_surface;
    bool m_frameReady;

    /** XKeyboard **/
    unsigned int *m_modmask;

    /** Private Methods **/
    SDL_Surface *GetFrameAsSurface();
    Window *findWindowByName(Window window);
    void RegisterWindowThread();
    void ResizeWindowThread(int w, int h);
    // Replace this with a generic event handler if needed
    bool MatchResizeEvent(int w, int h, XEvent *event);
    void QueueFrameThread();

    /** TCP **/
    static constexpr int TCP_PORT = 8080;
    int m_serverfd, m_latestsocket;
    struct sockaddr_in m_address;
    void InitializeTCPLayerThread();
    void SendToBufferThread();
    std::mutex m_tcpMutex;
};

#endif
