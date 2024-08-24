#ifndef RECT2D_HPP
#define RECT2D_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <IRender.hpp>
#include <GraphicsController.hpp>

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

    void GenBindBufferGL() override{
        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);
        glGenBuffers(1, &m_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glGenBuffers(1, &m_indexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);

        glBufferData(
            GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(GLfloat),
            m_vertexData.data(),
            GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_indexData.size() * sizeof(GLuint), m_indexData.data(),
                     GL_STATIC_DRAW);

    }

  public:
    Rect2D(GLfloat xPos = 0, GLfloat yPos = 0, GLfloat width = 0, GLfloat height = 0) {
        m_x = xPos;
        m_y = yPos;
        m_width = width;
        m_height = height;
        m_texture_width = 0;
        m_texture_height = 0;
        UpdateVertexData();

        m_indexData = {2, 0, 1, 3, 2, 1};
        UpdateGL();
    }

    virtual void UpdateVertexData(){
        m_vertexData = {
            m_x, m_y + m_height, m_x + m_width, m_y + m_height, m_x, m_y, m_x + m_width,
            m_y};
        UpdateGL();
    }

    void SetPosition(GLfloat xPos, GLfloat yPos) {
        m_x = xPos;
        m_y = yPos;

        UpdateVertexData();
    }

    void SetSize(GLfloat width, GLfloat height) {
        m_width = width;
        m_height = height;

        UpdateVertexData();
    }

    [[nodiscard]] GLfloat GetX() const { return m_x; }
    [[nodiscard]] GLfloat GetY() const { return m_y; }
    [[nodiscard]] GLfloat GetWidth() const { return m_width; }
    [[nodiscard]] GLfloat GetHeight() const { return m_height; }

    [[nodiscard]] std::vector<GLfloat> GetVertexData() const {
        return m_vertexData;
    }

    ~Rect2D() {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        glDeleteBuffers(1, &m_vertexBufferObject);
        glDeleteBuffers(1, &m_indexBufferObject);
    }

    void Render() override {
        glBindVertexArray(m_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glDrawElements(GL_TRIANGLES, RECT_ELEMENTS, GL_UNSIGNED_INT, nullptr);
        glUseProgram(0);
    }

  private:
    void UpdateGL() override {
        GenBindBufferGL();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              sizeof(GL_FLOAT) * 2, (void *)nullptr);

        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    static constexpr int RECT_ELEMENTS = 6;
};

#endif
