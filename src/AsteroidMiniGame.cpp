#include <AsteroidMiniGame.hpp>
#include <iostream>
#include <random>
#include <sstream>

AsteroidMiniGame::AsteroidMiniGame(App &app,
                                   std::shared_ptr<VimEmulator> vimEmulator)
    : app(app) {
    m_elapsed = 0.0;
    m_vimEmulator = vimEmulator;
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_vimEmulator->StartBufferReciever();
}

AsteroidMiniGame::~AsteroidMiniGame() { m_vimEmulator->StopBufferReciever(); }

void AsteroidMiniGame::Run() {
    m_elapsed += app.DeltaTime();

    if (m_vimEmulator->IsRequestReady()) {
        ParseRequest(m_vimEmulator->GetRequest());
    }

    if (m_elapsed > TICK_DELAY_MS) {
        UpdateAsteroids();
        m_elapsed = std::fmod(m_elapsed, TICK_DELAY_MS);
        m_vimEmulator->SendToBuffer(RenderGameState());
    }
}

std::string AsteroidMiniGame::RenderGameState() {
    std::string result = "";
    for (int asteroid : m_asteroids) {
        if (asteroid > 0) {
            result.append(asteroid, ' ');
            result.append("*");
            result.append(ROW_WIDTH - asteroid, ' ');
        } else {
            result.append(ROW_WIDTH, ' ');
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
            if (rand() % 100 < 30) {
                asteroid = 8;
            }
        }
    }
}

void AsteroidMiniGame::ParseRequest(std::string request) {
    if (request.length() < 1) {
        return;
    }

    request.pop_back();
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
