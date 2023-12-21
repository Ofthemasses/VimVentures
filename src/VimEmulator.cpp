#include "VimEmulator.hpp"
#include <iostream>
#include <unistd.h>

VimEmulator::VimEmulator(const winsize &ws): m_winsize(ws){
    pid_t pid = forkpty(&m_amaster, 0, 0, &m_winsize);
    if (pid < 0){
        std::cout << "Error Creating Terminal" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0){
        execlp("vim", "vim", NULL);
        // If execlp fails fail
        std::cout << "vim failed to execute" << std::endl;
        exit(EXIT_FAILURE);
    }
}

VimEmulator::~VimEmulator(){}

void VimEmulator::Run(){}

void VimEmulator::Print(){
    std::cout << m_outputBuffer << std::endl;
}

ssize_t VimEmulator::Read(){
    ssize_t bytesRead = read(m_amaster, m_outputBuffer, sizeof(m_outputBuffer) -1);
    if (bytesRead < 0) {
        std::cout << "Error Reading Terminal" << std::endl;
        exit(EXIT_FAILURE);
    }
    m_outputBuffer[bytesRead] = '\0';
    close(m_amaster);
    return bytesRead;
}

bool VimEmulator::Ready(){
    ssize_t bytesRead = read(m_amaster, m_outputBuffer, sizeof(m_outputBuffer) -1);
    return bytesRead > 20; // This is a guess 
}
