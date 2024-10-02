#ifndef TUTORIALMISSION_HPP
#define TUTORIALMISSION_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <VimEmulator.hpp>
#include <memory>
#include <queue>

class TutorialMission : public IMission {
    App &app;

  public:
    TutorialMission(App &app, std::shared_ptr<VimEmulator> vimEmulator);
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
