#include <AsteroidMiniGame.hpp>
#include <BasicMiniGame.hpp>
#include <DPLineMiniGame.hpp>
#include <SDL2/SDL_image.h>
#include <TutorialMission.hpp>

TutorialMission::TutorialMission(App &app, MissionState &missionState,
                                 std::shared_ptr<VimEmulator> vimEmulator)
    : app(app), missionState(missionState) {
    m_bar = 0.0;
    m_gameId = 0;
    m_vimEmulator = vimEmulator;
    m_currentMiniGame =
        std::make_unique<BasicMiniGame>(app, *this, m_vimEmulator);

    // Setup bar
    m_barGraphic = IMG_Load("./assets/images/TutorialBar.png");

    m_barRect =
        std::make_shared<TexturedRect2D>(0.4, -0.4, BAR_WIDTH, BAR_HEIGHT);
    m_barRect->UpdateVertexData();
    m_barRect->SetTextureFormat(GL_RGB);
    m_barRect->SetTexture(m_barGraphic->pixels, m_barGraphic->w,
                          m_barGraphic->h);
    m_barRect->SetShaderProgram("sp_gradient");
    TexturedRect2D::UniformVariableValue value;
    value.uniform1f = 0.0;

    m_barRect->AddUniformVariable("u_Fill", value, "glUniform1f");
    app.AddRenderable(m_barRect);
    m_vimEmulator->StartBufferReciever();
}

TutorialMission::~TutorialMission() { SDL_FreeSurface(m_barGraphic); 
m_vimEmulator->StopBufferReciever();}

void TutorialMission::Run() { 
    if (m_bar > 99.0){
        m_gameId++;
        m_bar = 0.0;
        switch(m_gameId){
            case 1:
                m_currentMiniGame = std::make_unique<DPLineMiniGame>(app, *this, m_vimEmulator);
                break;
            case 2:
                m_currentMiniGame = std::make_unique<AsteroidMiniGame>(app, *this, m_vimEmulator);
                break;
            case 3:
                m_vimEmulator->SendToBuffer(DEMO_MESSAGE);
                break;
        }
    }
    if (m_gameId < GAME_COUNT) {
        m_currentMiniGame->Run(); 
    }
}

void TutorialMission::UptickBar(float amount) {
    m_bar += amount;
    TexturedRect2D::UniformVariableValue value;
    value.uniform1f = m_bar / 100.0;

    m_barRect->SetUniformVariable("u_Fill", value);
}

void TutorialMission::DowntickBar(float amount) {
    if (amount > m_bar) {
        m_bar = 0;
    } else {
        m_bar -= amount;
        m_bar = m_bar < 0 ? 0 : m_bar;
    }
    TexturedRect2D::UniformVariableValue value;
    value.uniform1f = m_bar / 100.0;

    m_barRect->SetUniformVariable("u_Fill", value);
}

void TutorialMission::SetBar(float amount) {
    if (amount < 0.0){
        return;
    }
    m_bar = amount;
    TexturedRect2D::UniformVariableValue value;
    value.uniform1f = m_bar / 100.0;

    m_barRect->SetUniformVariable("u_Fill", value);
}

void TutorialMission::SetRestrictionText(std::string restrictions) {
    missionState.UpdateHelperMonitor(RESTRICT_HEADER + restrictions);
}
