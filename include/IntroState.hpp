#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "App.hpp"
#include "IState.hpp"
#include <TexturedRect2D.hpp>
#include <memory>

class IntroState : public IState {
    App &app;

  public:
    IntroState(App &app);
    ~IntroState() override;
    void Run() override;
    void SendEvent(SDL_Event &event) override;

  private:
    struct Slide {
        SDL_Surface *slideSurface;
        float startX;
        float stopX;
        float startY;
        float stopY;
        float panSpeedMS;
    };

    std::shared_ptr<TexturedRect2D> m_slideshow;
    Slide m_slides[3];
    int m_currSlide;

    void NextSlide();

    static constexpr int SLIDE_COUNT = 3;
    static constexpr float SLIDE_RIGHT = -1.4 / 2;
    static constexpr float SLIDE_LEFT = -1.0 / 2;
    static constexpr float SLIDE_TOP = -1.4 / 2;
    static constexpr float SLIDE_BOTTOM = -1.0 / 2;
    static constexpr float SLIDE_MIDDLE = -1.2 / 2;
    static constexpr float SLIDE_ZOOM = 1.2;
    static constexpr float SECOND_MS = 1000.0;
};

#endif
