#include "Rect2D.hpp"

/**
 * Representation a 2D quad, an encapsulation of two GL Triangles.
 *
 * @param xPos X position
 * @param yPos Y position
 * @param width Width of the quad
 * @param height Height of the quad
 */
Rect2D::Rect2D(GLfloat xPos, GLfloat yPos, GLfloat width, GLfloat height) {
    m_x = xPos;
    m_y = yPos;
    m_width = width;
    m_height = height;
    m_texture_width = 0;
    m_texture_height = 0;
    m_indexData = {2, 0, 1, 3, 2, 1};
}

/**
 * Cleans up OpenGL vertex arrays and buffers on
 * destruction.
 */
Rect2D::~Rect2D() {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    glDeleteBuffers(1, &m_vertexBufferObject);
    glDeleteBuffers(1, &m_indexBufferObject);
}

/**
 * Generates vertex arrays and buffers. Binds
 * vertex arrays and buffers. Then initializes data stores
 * for the vertex and index data.
 */
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

/**
 * Updates the Vertex Data.
 *
 * @note This should be overridden if you want to provide more data in strides.
 *
 * There are four groups, describing the four points of the quad.
 * @code
 * void Rect2D::UpdateVertexData() {
 *    m_vertexData = {TL, BR, BL, TR}
 *    \/\/ Always UpdateGL after changing vertex data.
 *    UpdateGL();
 * }
 * @endcode
 *
 */
void Rect2D::UpdateVertexData() {
    m_vertexData = {m_x,           m_y + m_height,
                    m_x + m_width, m_y + m_height,
                    m_x,           m_y,
                    m_x + m_width, m_y};
    UpdateGL();
}

/**
 * Set the position and update the vertex data.
 *
 * @param xPos New x Position
 * @param yPos New y Position
 */
void Rect2D::SetPosition(GLfloat xPos, GLfloat yPos) {
    m_x = xPos;
    m_y = yPos;

    UpdateVertexData();
}

/**
 * Set the width and height of the quad and update the vertex data.
 *
 * @param width New quad width
 * @param height New quad height
 */
void Rect2D::SetSize(GLfloat width, GLfloat height) {
    m_width = width;
    m_height = height;

    UpdateVertexData();
}

/**
 * Get the X position of the quad.
 *
 * @return X position of the quad
 */
GLfloat Rect2D::GetX() const { return m_x; }

/**
 * Get the Y position of the quad.
 *
 * @return Y position of the quad
 */
GLfloat Rect2D::GetY() const { return m_y; }

/**
 * Get the width of the quad.
 *
 * @return Width of the quad
 */
GLfloat Rect2D::GetWidth() const { return m_width; }

/**
 * Get the height of the quad.
 *
 * @return Height of the quad
 */
GLfloat Rect2D::GetHeight() const { return m_height; }

/**
 * Get the raw vertex data.
 *
 * @return Vertex data as vector
 */
std::vector<GLfloat> Rect2D::GetVertexData() const { return m_vertexData; }

/**
 * Renders the quad.
 */
void Rect2D::Render() {
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glDrawElements(GL_TRIANGLES, RECT_ELEMENTS, GL_UNSIGNED_INT, nullptr);
    glUseProgram(0);
}

/**
 * Updates the vertex arrays in the OpenGL FSM.
 */
void Rect2D::UpdateGL() {
    GenBindBufferGL();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2,
                          (void *)nullptr);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
