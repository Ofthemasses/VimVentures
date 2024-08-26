#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "App.hpp"
#include "IState.hpp"

class MenuState : public IState {
    App &app;

  public:
    MenuState(App &app);
    ~MenuState() override;
    void Run() override;
    void SendEvent(SDL_Event &event) override;

  private:
};

#endif
