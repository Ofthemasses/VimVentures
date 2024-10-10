#include <AsteroidMiniGame.hpp>
#include <iostream>
#include <random>
#include <sstream>

AsteroidMiniGame::AsteroidMiniGame(App &app, TutorialMission &tutorialMission,
                                   std::shared_ptr<VimEmulator> vimEmulator)
    : app(app), tutorialMission(tutorialMission) {
    m_elapsed = 0.0;
    m_vimEmulator = vimEmulator;
    m_asteroids.emplace_back(-1);
    m_asteroids.emplace_back(-1);
    m_asteroids.emplace_back(-1);
    m_asteroids.emplace_back(-1);
    m_asteroids.emplace_back(-1);
    m_asteroids.emplace_back(-1);
    m_vimEmulator->ClearKeyWhiteList();

    // Restrictions for mini-game
    m_vimEmulator->RestrictDuplicateOps();
    m_vimEmulator->AddKeyWhiteList('j', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('k', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('d', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('0', KMOD_NONE);
    m_vimEmulator->AddKeyWhiteList('4', KMOD_SHIFT);

    tutorialMission.SetRestrictionText(
        "j k d 0 $ \n RESTRICTED: duplicate- operators");
}

AsteroidMiniGame::~AsteroidMiniGame() = default;

void AsteroidMiniGame::Run() {
    m_elapsed += app.DeltaTime();

    if (m_vimEmulator->IsRequestReady()) {
        ParseRequest(m_vimEmulator->GetRequest());
    }

    if (m_elapsed > TICK_DELAY_MS) {
        tutorialMission.UptickBar(UP_TICK);
        UpdateAsteroids();
        m_elapsed = std::fmod(m_elapsed, TICK_DELAY_MS);
        m_vimEmulator->SendToBuffer(RenderGameState());
    }
}

std::string AsteroidMiniGame::RenderGameState() {
    std::string result;
    for (int asteroid : m_asteroids) {
        switch (asteroid) {
        case 0:
            result.append("X");
            result.append(ROW_WIDTH - 1, ' ');
            tutorialMission.DowntickBar(DOWN_TICK);
            break;
        case -1:
            result.append(ROW_WIDTH, ' ');
            break;
        default:
            result.append(asteroid, ' ');
            result.append("*");
            result.append(ROW_WIDTH - asteroid, ' ');
            break;
        }
        result.append("\n");
    }
    return result;
}

void AsteroidMiniGame::UpdateAsteroids() {
    for (int &asteroid : m_asteroids) {
        if (asteroid > 0) {
            asteroid--;
        } else {
            if (rand() % RAND_RANGE < RAND_CHANCE) {
                asteroid = ASTEROID_START;
            }
        }
    }
}

void AsteroidMiniGame::ParseRequest(std::string request) {
    if (request.length() < 1) {
        return;
    }

    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(request);
    int asteroid = 0;

    while (std::getline(stream, temp, '\n')) {
        bool allSpaces =
            std::all_of(temp.begin(), temp.end(),
                        [](char character) { return character == ' '; });
        if (allSpaces) {
            m_asteroids[asteroid] = -1;
        }
        asteroid++;
    }
}
