#ifndef TUTORIALMISSION_HPP
#define TUTORIALMISSION_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <VimEmulator.hpp>
#include <TexturedRect2D.hpp>
#include <memory>
#include <queue>

class TutorialMission : public IMission {
    App &app;

  public:
    TutorialMission(App &app, std::shared_ptr<VimEmulator> vimEmulator);
    ~TutorialMission();
    void Run() override;

    /** MiniGame Handling **/
    void UptickBar(float amount);
    void DowntickBar(float amount);

  private:
    // Yes re using IMission here is a little strange.
    // But honestly a seperate MiniGame class will be
    // exactly the same as a IMission. Maybe IMission
    // should be renamed down the line.
    std::queue<IMission> m_miniGames;
    std::unique_ptr<IMission> m_currentMiniGame;

    std::shared_ptr<TexturedRect2D> m_barRect;
    SDL_Surface *m_barGraphic;
    float m_bar;

    static constexpr float BAR_HEIGHT = 0.2;
    static constexpr float BAR_WIDTH = 100.0/800.0 * 0.2;
};

#endif
