#ifndef MISSION_HPP
#define MISSION_HPP

#include "App.hpp"
#include "IState.hpp"
#include "VimEmulator.hpp"
#include <IMission.hpp>

class MissionState : public IState {
    App &app;

  public:
    MissionState(App &app);
    ~MissionState() override;
    void Run() override;
    void SendEvent(SDL_Event &event) override;
    void UpdateHelperMonitor(std::string text);

  private:
    static constexpr float WIDTH_RATIO = 2 / 3.0;
    static constexpr int DRAW_R = 131;
    static constexpr int DRAW_G = 50;
    static constexpr int DRAW_B = 168;

    static constexpr float HELPER_WIDTH = 1.0 / 8.0;
    static constexpr float HELPER_X_POSITION = -24.0 / 50.0;
    static constexpr int HELPER_TEXT_PADDING = 25;

    static constexpr float LEFT = -0.5;
    static constexpr float TOP = -0.5;

    static constexpr SDL_Color TEXT_WHITE = {255, 255, 255};

    std::shared_ptr<VimEmulator> m_vimEmulator;

    std::unique_ptr<IMission> m_currentMission;
    SDL_Surface *m_helpMonitorImage;
    SDL_Surface *m_helpMonitorSurface;

    std::shared_ptr<TexturedRect2D> m_helpMonitor;
};

#endif
