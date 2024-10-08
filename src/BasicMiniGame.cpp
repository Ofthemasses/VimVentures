#include <BasicMiniGame.hpp>
#include <iostream>
#include <random>
#include <sstream>

BasicMiniGame::BasicMiniGame(App &app, TutorialMission &tutorialMission,
                                   std::shared_ptr<VimEmulator> vimEmulator)
    : app(app), tutorialMission(tutorialMission) {
    m_elapsed = 0.0;
    m_vimEmulator = vimEmulator;
    m_gameState = "     +    +      \n"
                  "                 \n"
                  "              +  \n"
                  "  +              \n"
                  "                 \n"
                  "         +       \n"
                  "  +              \n"
                  "                 \n"
                  "                 \n"
                  "       +     +   \n";
    m_vimEmulator->SendToBuffer(m_gameState);
    m_vimEmulator->ClearKeyWhiteList();

    // Restrictions for mini-game
    m_vimEmulator->RestrictDuplicateOps();
    m_vimEmulator->AddKeyWhiteList('h', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('j', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('k', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('l', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('x', KMOD_NONE);

    tutorialMission.SetRestrictionText("h j k l x");
}

BasicMiniGame::~BasicMiniGame() = default;

void BasicMiniGame::Run() {
    m_elapsed += app.DeltaTime();

    if (m_vimEmulator->IsRequestReady()) {
        ParseRequest(m_vimEmulator->GetRequest());
    }
}

std::string BasicMiniGame::RenderGameState() {
    return m_gameState;
}

void BasicMiniGame::ParseRequest(std::string request) {
    if (request.length() < 1) {
        return;
    }
    m_gameState = request;
    int count = 0;
    for(char& c : request) {
        if (c == '+'){
            count++;
        }
    }
    tutorialMission.SetBar((1.0 - count/INITIAL_PLUSES) * 100.0);
}
