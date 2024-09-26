#ifndef TEXTUREDRECT2D_HPP
#define TEXTUREDRECT2D_HPP

#include <GraphicsController.hpp>
#include <Rect2D.hpp>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

class TexturedRect2D : public Rect2D {
  protected:
    GLuint m_texture;
    GLuint m_texture_width;
    GLuint m_texture_height;

  public:
    TexturedRect2D(GLfloat xPos = 0, GLfloat yPos = 0, GLfloat width = 0,
                   GLfloat height = 0);

    void UpdateVertexData() override;

    void SetTexture(void *data, GLuint width, GLuint height,
                    bool force = false);

    void SetTextureFormat(GLenum format);

    void SetInternalTextureFormat(GLenum format);

    void EnableTextureBlend();

    void DisableTextureBlend();

    ~TexturedRect2D();

    void Render() override;

    void UpdateGL() override;

  private:
    GLenum m_textureFormat;
    GLenum m_internalTextureFormat;
    bool m_enableBlend;
};

#endif
