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

  private:
    static constexpr float WIDTH_RATIO = 2 / 3.0;
    static constexpr int DRAW_R = 131;
    static constexpr int DRAW_G = 50;
    static constexpr int DRAW_B = 168;

    static constexpr float HELPER_WIDTH = 1.0 / 8.0;
    static constexpr float HELPER_X_POSITION = -24.0 / 50.0;

    std::shared_ptr<VimEmulator> m_vimEmulator;

    std::unique_ptr<IMission> m_currentMission;
};

#endif
