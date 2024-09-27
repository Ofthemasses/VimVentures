#ifndef MISSION_HPP
#define MISSION_HPP

#include "App.hpp"
#include "IState.hpp"
#include "VimEmulator.hpp"

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

    std::shared_ptr<VimEmulator> m_vimEmulator;
};

#endif
