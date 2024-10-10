#ifndef BASICMINIGAME_HPP
#define BASICMINIGAME_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <TutorialMission.hpp>
#include <VimEmulator.hpp>

class BasicMiniGame : public IMission {
    App &app;
    TutorialMission &tutorialMission;

  public:
    BasicMiniGame(App &app, TutorialMission &tutorialMission,
                  std::shared_ptr<VimEmulator> vimEmulator);
    ~BasicMiniGame() override;
    void Run() override;

  private:
    std::shared_ptr<VimEmulator> m_vimEmulator;
    static constexpr double TICK_DELAY_MS = 10.0;
    static constexpr float INITIAL_PLUSES = 8.0;
    double m_elapsed;
    std::string m_gameState;

    std::string RenderGameState();
    void ParseRequest(std::string request);
};

#endif
