#ifndef ASTEROIDMINIGAME_HPP
#define ASTEROIDMINIGAME_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <VimEmulator.hpp>
#include <TutorialMission.hpp>

class AsteroidMiniGame : public IMission {
    App &app;
    TutorialMission &tutorialMission;

  public:
    AsteroidMiniGame(App &app, TutorialMission &tutorialMission, std::shared_ptr<VimEmulator> vimEmulator);
    ~AsteroidMiniGame() override;
    void Run() override;

  private:
    std::shared_ptr<VimEmulator> m_vimEmulator;
    static constexpr double TICK_DELAY_MS = 10.0;
    static constexpr int ROW_WIDTH = 12;
    double m_elapsed;
    std::vector<int> m_asteroids;

    std::string RenderGameState();
    void UpdateAsteroids();
    void ParseRequest(std::string request);
};

#endif
