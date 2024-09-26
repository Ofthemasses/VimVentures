#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "App.hpp"
#include "IState.hpp"
#include "TexturedRect2D.hpp"
#include <memory>

class MenuState : public IState {
    App &app;

  public:
    MenuState(App &app);
    ~MenuState() override;
    void Run() override;
    void SendEvent(SDL_Event &event) override;

  private:
    std::shared_ptr<TexturedRect2D> m_titleText;

    // Title Variables
    static constexpr SDL_Color TITLE_COLOR = {255, 255, 255};
    static constexpr float TITLE_W_SCALE = 0.001;
    static constexpr float TITLE_H_SCALE = 0.001;
    static constexpr float TITLE_X_OFFSET = 0.0;
    static constexpr float TITLE_Y_OFFSET = 1.0 / 4.0;
};

#endif
