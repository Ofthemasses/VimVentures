#ifndef TEXTUREDRECT2D_HPP
#define TEXTUREDRECT2D_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <Rect2D.hpp>
#include <GraphicsController.hpp>

class TexturedRect2D : public Rect2D {
  protected:
    GLuint m_texture;
    GLuint m_texture_width;
    GLuint m_texture_height;

  public:
    TexturedRect2D(GLfloat xPos = 0, GLfloat yPos = 0, GLfloat width = 0, GLfloat height = 0) : Rect2D(xPos,yPos,width,height) {}

    void UpdateVertexData() override {
        m_vertexData = {m_x,           m_y + m_height,
                        0.0F,          0.0F,
                        m_x + m_width, m_y + m_height,
                        1.0F,          0.0F,
                        m_x,           m_y,
                        0.0F,          1.0F,
                        m_x + m_width, m_y,
                        1.0F,          1.0F};
    }

    void SetTexture(void *data, GLuint width, GLuint height, bool force = false) {
        if (width == m_texture_width &&
            height == m_texture_height &&
            !force) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                            GL_UNSIGNED_BYTE, data);
            return;
        }

        if (glIsTexture(m_texture)){
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

    ~TexturedRect2D() {
        glDeleteTextures(1, &m_texture);
    }

    void Render() override {
        glUseProgram(GraphicsController::s_shaderPrograms.at("sp_cathode")->GetProgramId());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        GLint textureLocation =
            glGetUniformLocation(GraphicsController::s_shaderPrograms.at("sp_cathode")->GetProgramId(), "u_Texture");
        glUniform1i(textureLocation, 0);

        Rect2D::Render();
    }

  private:
    void UpdateGL() override {
        GenBindBufferGL();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              sizeof(GL_FLOAT) * 4, (void *)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              sizeof(GL_FLOAT) * 4,
                              (GLvoid *)(sizeof(GL_FLOAT) * 2));
        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
};

#endif
