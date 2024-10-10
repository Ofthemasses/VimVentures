#ifndef ASTEROIDMINIGAME_HPP
#define ASTEROIDMINIGAME_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <TutorialMission.hpp>
#include <VimEmulator.hpp>

class AsteroidMiniGame : public IMission {
    App &app;
    TutorialMission &tutorialMission;

  public:
    AsteroidMiniGame(App &app, TutorialMission &tutorialMission,
                     std::shared_ptr<VimEmulator> vimEmulator);
    ~AsteroidMiniGame() override;
    void Run() override;

  private:
    std::shared_ptr<VimEmulator> m_vimEmulator;
    static constexpr double TICK_DELAY_MS = 10.0;
    static constexpr int ROW_WIDTH = 12;
    static constexpr int UP_TICK = 5;
    static constexpr int DOWN_TICK = 10;
    static constexpr int ASTEROID_START = 8;
    static constexpr size_t RAND_RANGE = 100;
    static constexpr size_t RAND_CHANCE = 30;
    double m_elapsed;
    std::vector<int> m_asteroids;

    std::string RenderGameState();
    void UpdateAsteroids();
    void ParseRequest(std::string request);
};

#endif
