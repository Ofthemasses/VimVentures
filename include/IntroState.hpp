#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP 

#include "App.hpp"
#include "IState.hpp"
#include <memory>
#include <TexturedRect2D.hpp>

class IntroState : public IState {
    App &app;

  public:
    IntroState(App &app);
    ~IntroState() override;
    void Run() override;
    void SendEvent(SDL_Event &event) override;

  private:
    SDL_Surface* m_slides[1];
    std::shared_ptr<TexturedRect2D> m_slideshow;
};

#endif
