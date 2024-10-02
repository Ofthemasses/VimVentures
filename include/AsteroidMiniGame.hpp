#ifndef ASTEROIDMINIGAME_HPP
#define ASTEROIDMINIGAME_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <VimEmulator.hpp>

class AsteroidMiniGame : public IMission {
    App &app;

  public:
    AsteroidMiniGame(App &app, std::shared_ptr<VimEmulator> vimEmulator);
    void Run() override;

  private:
    std::shared_ptr<VimEmulator> m_vimEmulator;
    static constexpr double TICK_DELAY_MS = 10.0;
    static constexpr int ROW_WIDTH = 12;
    double m_elapsed;
    std::vector<int> m_asteroids;

    std::string RenderGameState();
    void UpdateAsteroids();
};

#endif
