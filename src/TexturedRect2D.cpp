#include "TexturedRect2D.hpp"

TexturedRect2D::TexturedRect2D(GLfloat xPos, GLfloat yPos, GLfloat width,
                               GLfloat height)
    : Rect2D(xPos, yPos, width, height) {}

TexturedRect2D::~TexturedRect2D() { glDeleteTextures(1, &m_texture); }

void TexturedRect2D::UpdateVertexData() {
    m_vertexData = {m_x,           m_y + m_height,
                    0.0F,          0.0F,
                    m_x + m_width, m_y + m_height,
                    1.0F,          0.0F,
                    m_x,           m_y,
                    0.0F,          1.0F,
                    m_x + m_width, m_y,
                    1.0F,          1.0F};
}

void TexturedRect2D::SetTexture(void *data, GLuint width, GLuint height,
                                bool force) {
    if (width == m_texture_width && height == m_texture_height && !force) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                        GL_UNSIGNED_BYTE, data);
        return;
    }

    if (glIsTexture(m_texture)) {
        glDeleteTextures(1, &m_texture);
    }

    m_texture_width = width;
    m_texture_height = height;
    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    UpdateGL();
}

void TexturedRect2D::Render() {
    glUseProgram(
        GraphicsController::s_shaderPrograms.at("sp_cathode")->GetProgramId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    GLint textureLocation = glGetUniformLocation(
        GraphicsController::s_shaderPrograms.at("sp_cathode")->GetProgramId(),
        "u_Texture");
    glUniform1i(textureLocation, 0);

    Rect2D::Render();
}

void TexturedRect2D::UpdateGL() {
    GenBindBufferGL();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4,
                          (void *)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4,
                          (GLvoid *)(sizeof(GL_FLOAT) * 2));
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
