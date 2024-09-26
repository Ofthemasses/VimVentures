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
    static constexpr SDL_Color TEXT_COLOR = {255, 255, 255};

    // Title Variables
    std::shared_ptr<TexturedRect2D> m_titleText;
    static constexpr float TITLE_W_SCALE = 0.001;
    static constexpr float TITLE_H_SCALE = 0.001;
    static constexpr float TITLE_X_OFFSET = 0.0;
    static constexpr float TITLE_Y_OFFSET = 1.0 / 4.0;

    // Menu Variables
    static constexpr float MENU_W_SCALE = 0.0004;
    static constexpr float MENU_H_SCALE = 0.0004;

    std::shared_ptr<TexturedRect2D> m_startText;
    static constexpr float START_X_OFFSET = 0.0;
    static constexpr float START_Y_OFFSET = 0.0;

    std::shared_ptr<TexturedRect2D> m_quitText;
    static constexpr float QUIT_X_OFFSET = 0.0;
    static constexpr float QUIT_Y_OFFSET = -1.0 / 8.0;

    static constexpr int MENU_COUNT = 2;

    // Selector
    std::shared_ptr<TexturedRect2D> m_selector;
    int selector_pos;
    static constexpr float SELECTOR_PADDING = 1.0 / 32.0;
    void SelectorNext();
    void SelectorPrev();
    void Select();

    enum MenuOption { START = 0, QUIT = 1 };
};

#endif
