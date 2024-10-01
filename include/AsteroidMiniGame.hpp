#ifndef ASTEROIDMINIGAME_HPP
#define ASTEROIDMINIGAME_HPP

#include <IMission.hpp>
#include <VimEmulator.hpp>

class AsteroidMiniGame : public IMission {
    public:
        AsteroidMiniGame(std::shared_ptr<VimEmulator> vimEmulator);
        void Run() override;
    private:
        std::shared_ptr<VimEmulator> m_vimEmulator;

};

#endif
