#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP
#include <IMission.hpp>
#include <SDL2/SDL.h>
#include <X11/Xlib.h>
#include <functional>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <thread>

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
    void SendToBuffer(std::string_view message);
    void StartBufferReciever();
    [[nodiscard]] bool IsRequestReady() const;
    [[nodiscard]] std::string GetRequest();
    void StopBufferReciever();

    /** X11 Keyboard **/
    void SendSDLKey(SDL_Keycode key);
    void SetSDLMod(Uint16 mod);

    /** Key Restrictions **/
    void ClearKeyWhiteList();
    void AddKeyWhiteList(SDL_Keycode, Uint16);
    void RestrictDuplicateOps();
    void AllowDuplicateOps();

    /** IRender **/
    void Render() override;

  private:
    static constexpr int REFRESH_MS = 100;
    static constexpr size_t THREAD_H_PRIORITY = 10;
    static constexpr size_t THREAD_M_PRIORITY = 5;
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

    /** Key Restrictions **/
    std::vector<std::pair<SDL_Keycode, Uint16>> m_whiteList;
    std::pair<SDL_Keycode, Uint16> m_prevKey;
    bool m_restrictDuplicateOps;
    bool isDuplicateOp(SDL_Keycode keyCode);

    SDL_Surface *GetFrameAsSurface();
    Window *findWindowByName(Window window);
    void RegisterWindowThread();
    void ResizeWindowThread(int w, int h);
    // Replace this with a generic event handler if needed
    bool MatchResizeEvent(int w, int h, XEvent *event);
    void QueueFrameThread();
    static void SetThreadPriority(std::thread &thread, int priority);

    /** TCP **/
    static constexpr int TCP_PORT = 8080;
    static constexpr size_t SEND_SIG_BUFFER_SIZE = 256;
    static constexpr size_t RECIEVE_BUFFER_SIZE = 2048;
    int m_serverfd, m_latestsocket;
    struct sockaddr_in m_address;
    void InitializeTCPLayerThread();
    void SendToBufferThread(std::string message);
    void BufferRecieverThread();
    std::mutex m_tcpMutex;
    bool m_requestReady;
    bool m_recievingBuffer;
    std::string m_requestResult;
};
#endif
