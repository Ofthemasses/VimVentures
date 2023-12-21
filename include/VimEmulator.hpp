#ifndef VIMEMULATOR_HPP
#define VIMEMULATOR_HPP

#include <pty.h>
#include <sys/types.h>

class VimEmulator{
    public:
        VimEmulator(const winsize &ws);
        ~VimEmulator();
        void Run();
        void Print();
        ssize_t Read();
        bool Ready();
    private:
        char m_outputBuffer[4096];
        int m_amaster;
        winsize m_winsize;
};

#endif
