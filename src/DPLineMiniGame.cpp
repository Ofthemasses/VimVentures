#include <DPLineMiniGame.hpp>
#include <iostream>
#include <random>
#include <sstream>

DPLineMiniGame::DPLineMiniGame(App &app, TutorialMission &tutorialMission,
                                   std::shared_ptr<VimEmulator> vimEmulator)
    : app(app), tutorialMission(tutorialMission) {
    m_vimEmulator = vimEmulator;

    m_vimEmulator->SendToBuffer(INITIAL_GAME_STATE);
    m_vimEmulator->ClearKeyWhiteList();

    // Restrictions for mini-game
    m_vimEmulator->AllowDuplicateOps();
    m_vimEmulator->AddKeyWhiteList('d', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('p', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('j', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('k', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('u', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('r', KMOD_CTRL);

    tutorialMission.SetRestrictionText("j k d p u CTRL+r");
}

DPLineMiniGame::~DPLineMiniGame() = default;

void DPLineMiniGame::Run() {
    if (m_vimEmulator->IsRequestReady()) {
        ParseRequest(m_vimEmulator->GetRequest());
    }
}

void DPLineMiniGame::ParseRequest(std::string request) {
    if (request.length() < 1) {
        return;
    }

    request.pop_back();
    
    std::stringstream gameStringStream(request);
    std::string gameLine;
    size_t count = 0;
    size_t matches = 0;
    size_t extras = 0;
    while (std::getline(gameStringStream, gameLine, '\n')){
        if (count >= LINES){
            extras++;
            continue;
        }
        if (gameLine == m_targetState[count]){
            matches++;
        }
        count++;
    }

    tutorialMission.SetBar(100.0 * (float) matches / (LINES + extras));
}
