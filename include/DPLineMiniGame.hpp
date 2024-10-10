#ifndef DPLINEMINIGAME_HPP
#define DPLINEMINIGAME_HPP

#include <App.hpp>
#include <IMission.hpp>
#include <TutorialMission.hpp>
#include <VimEmulator.hpp>

class DPLineMiniGame : public IMission {
    App &app;
    TutorialMission &tutorialMission;

  public:
    DPLineMiniGame(App &app, TutorialMission &tutorialMission,
                     std::shared_ptr<VimEmulator> vimEmulator);
    ~DPLineMiniGame() override;
    void Run() override;

  private:
    std::shared_ptr<VimEmulator> m_vimEmulator;
    static constexpr double TICK_DELAY_MS = 10.0;
    // https://andrecox.github.io/ASCII-Text-Generator/
    const std::vector<std::string_view> m_targetState = {
		R"($$\                                 $$\                                   $$\ )",
		R"($$ |                                $$ |                                  $$ |)",
		R"($$ |$$\    $$\  $$$$$$\  $$$$$$$\ $$$$$$\   $$\   $$\  $$$$$$\   $$$$$$\  $$ |)",
		R"(\__|\$$\  $$  |$$  __$$\ $$  __$$\\_$$  _|  $$ |  $$ |$$  __$$\ $$  __$$\ \__|)",
		R"($$\  \$$\$$  / $$$$$$$$ |$$ |  $$ | $$ |    $$ |  $$ |$$ |  \__|$$$$$$$$ |$$\ )",
		R"($$ |  \$$$  /  $$   ____|$$ |  $$ | $$ |$$\ $$ |  $$ |$$ |      $$   ____|$$ |)",
		R"($$ |   \$  /   \$$$$$$$\ $$ |  $$ | \$$$$  |\$$$$$$  |$$ |      \$$$$$$$\ $$ |)",
		R"(\__|    \_/     \_______|\__|  \__|  \____/  \______/ \__|       \_______|\__|)"
    };
    static constexpr std::string_view INITIAL_GAME_STATE = 
		R"(\__|    \_/     \_______|\__|  \__|  \____/  \______/ \__|       \_______|\__|)" "\n"
		R"($$ |   \$  /   \$$$$$$$\ $$ |  $$ | \$$$$  |\$$$$$$  |$$ |      \$$$$$$$\ $$ |)" "\n"
		R"(\__|\$$\  $$  |$$  __$$\ $$  __$$\\_$$  _|  $$ |  $$ |$$  __$$\ $$  __$$\ \__|)" "\n"
		R"($$ |$$\    $$\  $$$$$$\  $$$$$$$\ $$$$$$\   $$\   $$\  $$$$$$\   $$$$$$\  $$ |)" "\n"
		R"($$\  \$$\$$  / $$$$$$$$ |$$ |  $$ | $$ |    $$ |  $$ |$$ |  \__|$$$$$$$$ |$$\ )" "\n"
		R"($$ |                                $$ |                                  $$ |)" "\n"
		R"($$ |  \$$$  /  $$   ____|$$ |  $$ | $$ |$$\ $$ |  $$ |$$ |      $$   ____|$$ |)" "\n"
		R"($$\                                 $$\                                   $$\ )"; 
    static constexpr size_t LINES = 8;

    void ParseRequest(std::string request);
};

#endif
