#include <TutorialMission.hpp>
#include <AsteroidMiniGame.hpp>

TutorialMission::TutorialMission(std::shared_ptr<VimEmulator> vimEmulator){
    m_currentMiniGame = std::make_unique<AsteroidMiniGame>(vimEmulator);
}

void TutorialMission::Run(){
    m_currentMiniGame->Run();
}
