#include "Rect2D.hpp"

Rect2D::Rect2D(GLfloat xPos, GLfloat yPos, GLfloat width, GLfloat height) {
    m_x = xPos;
    m_y = yPos;
    m_width = width;
    m_height = height;
    m_texture_width = 0;
    m_texture_height = 0;
    m_indexData = {2, 0, 1, 3, 2, 1};
}

Rect2D::~Rect2D() {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    glDeleteBuffers(1, &m_vertexBufferObject);
    glDeleteBuffers(1, &m_indexBufferObject);
}

void Rect2D::GenBindBufferGL() {
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);

    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(GLfloat),
                 m_vertexData.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(GLuint),
                 m_indexData.data(), GL_STATIC_DRAW);
}

void Rect2D::UpdateVertexData() {
    m_vertexData = {m_x,           m_y + m_height,
                    m_x + m_width, m_y + m_height,
                    m_x,           m_y,
                    m_x + m_width, m_y};
    UpdateGL();
}

void Rect2D::SetPosition(GLfloat xPos, GLfloat yPos) {
    m_x = xPos;
    m_y = yPos;

    UpdateVertexData();
}

void Rect2D::SetSize(GLfloat width, GLfloat height) {
    m_width = width;
    m_height = height;

    UpdateVertexData();
}

GLfloat Rect2D::GetX() const { return m_x; }
GLfloat Rect2D::GetY() const { return m_y; }
GLfloat Rect2D::GetWidth() const { return m_width; }
GLfloat Rect2D::GetHeight() const { return m_height; }

std::vector<GLfloat> Rect2D::GetVertexData() const { return m_vertexData; }

void Rect2D::Render() {
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glDrawElements(GL_TRIANGLES, RECT_ELEMENTS, GL_UNSIGNED_INT, nullptr);
    glUseProgram(0);
}

void Rect2D::UpdateGL() {
    GenBindBufferGL();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2,
                          (void *)nullptr);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
