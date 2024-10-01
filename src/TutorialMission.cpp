#include <TutorialMission.hpp>
#include <AsteroidMiniGame.hpp>

TutorialMission::TutorialMission(App &app, std::shared_ptr<VimEmulator> vimEmulator) : app(app){
    m_currentMiniGame = std::make_unique<AsteroidMiniGame>(app, vimEmulator);
}

void TutorialMission::Run(){
    m_currentMiniGame->Run();
}
