#ifndef RECT2D_HPP
#define RECT2D_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

class Rect2D {
  protected:
    std::vector<GLfloat> m_vertexData;
    std::vector<GLuint> m_indexData;
    GLuint m_texture;
    GLfloat m_x;
    GLfloat m_y;
    GLfloat m_width;
    GLfloat m_height;

    GLuint m_texture_width;
    GLuint m_texture_height;

    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    GLuint m_indexBufferObject;

    bool m_has_texture;

  public:
    Rect2D(GLfloat x = 0, GLfloat y = 0, GLfloat width = 0, GLfloat height = 0)
        : m_has_texture(false) {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_texture_width = 0;
        m_texture_height = 0;
        m_vertexData = {x, y + height, x + width, y + height,
                        x, y,          x + width, y};

        m_indexData = {2, 0, 1, 3, 2, 1};
        updateGL();
    }

    void SetPosition(GLfloat x, GLfloat y) {
        m_x = x;
        m_y = y;

        if (m_has_texture) {
            m_has_texture = false;
        } else {
            m_vertexData = {
                x, y + m_height, x + m_width, y + m_height, x, y, x + m_width,
                y};
        }

        updateGL();
    }

    void SetSize(GLfloat width, GLfloat height) {
        m_width = width;
        m_height = height;

        std::cout << m_x + width << std::endl;
        if (m_has_texture) {
            m_has_texture = false;
        } else {
            m_vertexData = {m_x, m_y + height, m_x + width, m_y + height,
                            m_x, m_y,          m_x + width, m_y};
        }
        updateGL();
    }

    void SetTexture(void *data, GLuint width, GLuint height) {
        if (m_has_texture && width == m_texture_width &&
            height == m_texture_height) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                            GL_UNSIGNED_BYTE, data);
            return;
        }
        m_vertexData = {m_x,           m_y + m_height,
                        0.0F,          0.0F,
                        m_x + m_width, m_y + m_height,
                        1.0F,          0.0F,
                        m_x,           m_y,
                        0.0F,          1.0F,
                        m_x + m_width, m_y,
                        1.0F,          1.0F};

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

        m_has_texture = true;
        updateGL();
    }

    [[nodiscard]] GLfloat GetX() const { return m_x; }
    [[nodiscard]] GLfloat GetY() const { return m_y; }
    [[nodiscard]] GLfloat GetWidth() const { return m_width; }
    [[nodiscard]] GLfloat GetHeight() const { return m_height; }

    [[nodiscard]] std::vector<GLfloat> GetVertexData() const {
        return m_vertexData;
    }

    virtual ~Rect2D() {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        glDeleteBuffers(1, &m_vertexBufferObject);
        glDeleteBuffers(1, &m_indexBufferObject);
        if (m_has_texture) {
            glDeleteTextures(1, &m_texture);
        }
    }

    void Draw(GLuint shaderProgram) {
        if (m_has_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture);
            // You should also set your uniform for the texture here
            GLint textureLocation =
                glGetUniformLocation(shaderProgram, "u_Texture");
            glUniform1i(textureLocation, 0);
        }

        glBindVertexArray(m_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
    }

  private:
    void updateGL() {
        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);
        glGenBuffers(1, &m_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glGenBuffers(1, &m_indexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);

        glBufferData(
            GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(GLfloat),
            m_vertexData.data(),
            GL_STATIC_DRAW); // Allocate buffer but don't upload data yet
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_indexData.size() * sizeof(GLuint), m_indexData.data(),
                     GL_STATIC_DRAW);

        if (m_has_texture) {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(GL_FLOAT) * 4, (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(GL_FLOAT) * 4,
                                  (GLvoid *)(sizeof(GL_FLOAT) * 2));
        } else {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(GL_FLOAT) * 2, (void *)0);
        }

        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
};

#endif
