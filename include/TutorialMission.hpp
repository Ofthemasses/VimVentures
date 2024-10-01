#ifndef TUTORIALMISSION_HPP
#define TUTORIALMISSION_HPP

#include <IMission.hpp>
#include <VimEmulator.hpp>
#include <queue>
#include <memory>

class TutorialMission : public IMission {
    public:
        TutorialMission(std::shared_ptr<VimEmulator> vimEmulator);
        void Run() override;

    private:
        // Yes re using IMission here is a little strange.
        // But honestly a seperate MiniGame class will be
        // exactly the same as a IMission. Maybe IMission
        // should be renamed down the line.
        std::queue<IMission> m_miniGames;
        std::unique_ptr<IMission> m_currentMiniGame;
};

#endif
