#include <AsteroidMiniGame.hpp>
#include <random>
#include <iostream>

AsteroidMiniGame::AsteroidMiniGame(App& app, std::shared_ptr<VimEmulator> vimEmulator) : app(app){
    m_elapsed = 0.0;
    m_vimEmulator = vimEmulator;
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
    m_asteroids.emplace_back(0);
}

void AsteroidMiniGame::Run() {
    m_elapsed += app.DeltaTime();

    if (m_elapsed > TICK_DELAY_MS){
        UpdateAsteroids();
        m_elapsed = std::fmod(m_elapsed, TICK_DELAY_MS);
        m_vimEmulator->SendToBuffer(RenderGameState());
    }
}

std::string AsteroidMiniGame::RenderGameState() {
    std::string result = "";
    for (int asteroid : m_asteroids){
        if (asteroid > 0){
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
    for (int &asteroid : m_asteroids){
        if (asteroid > 0){
            asteroid--;
        } else {
            if (rand() % 100 < 30){
                asteroid = 8;
            }
        }
    }
}
