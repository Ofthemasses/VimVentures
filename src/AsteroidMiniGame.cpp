#include <AsteroidMiniGame.hpp>

AsteroidMiniGame::AsteroidMiniGame(std::shared_ptr<VimEmulator> vimEmulator) {
    m_vimEmulator = vimEmulator;
}

void AsteroidMiniGame::Run() {
    m_vimEmulator->SendToBuffer("ASTEROIDS!!!"); 
}
