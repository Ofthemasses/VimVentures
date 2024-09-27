#ifndef RECT2D_HPP
#define RECT2D_HPP

#include <GraphicsController.hpp>
#include <IRender.hpp>
#include <SDL2/SDL.h>
#include <ShaderProgram.hpp>
#include <glad/glad.h>
#include <vector>

class Rect2D : public IRender {
  protected:
    std::vector<GLfloat> m_vertexData;
    std::vector<GLuint> m_indexData;
    GLfloat m_x;
    GLfloat m_y;
    GLfloat m_width;
    GLfloat m_height;

    GLuint m_texture_width;
    GLuint m_texture_height;

    void GenBindBufferGL() override;

  public:
    Rect2D(GLfloat xPos = 0, GLfloat yPos = 0, GLfloat width = 0,
           GLfloat height = 0);

    virtual void UpdateVertexData();

    void SetPosition(GLfloat xPos, GLfloat yPos);

    void SetSize(GLfloat width, GLfloat height);

    [[nodiscard]] GLfloat GetX() const;
    [[nodiscard]] GLfloat GetY() const;
    [[nodiscard]] GLfloat GetWidth() const;
    [[nodiscard]] GLfloat GetHeight() const;

    [[nodiscard]] std::vector<GLfloat> GetVertexData() const;

    ~Rect2D();

    void Render() override;

    void UpdateGL() override;

    void SetShaderProgram(std::string shaderProgram);

  private:
    static constexpr int RECT_ELEMENTS = 6;
};

#endif
