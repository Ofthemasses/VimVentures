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
    struct Slide {
        SDL_Surface* slideSurface;
        float startX;
        float stopX;
        float startY;
        float stopY;
        float panSpeedMS;
    };

    std::shared_ptr<TexturedRect2D> m_slideshow;
    Slide m_slides[2];
    int m_currSlide;

    void NextSlide();

    static constexpr int SLIDE_COUNT = 2; 
};

#endif
