#ifndef MISSION_HPP
#define MISSION_HPP

#include "IState.hpp"
#include "App.hpp"

class MissionState : public IState {
    App& app;

    public:
        MissionState(App& app);
        ~MissionState();
        void Run() override;
        void SendEvent(SDL_Event& event) override;

    private:
        VimEmulator* m_vimEmulator;
};

#endif
