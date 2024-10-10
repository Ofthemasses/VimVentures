#ifndef TEXTUREDRECT2D_HPP
#define TEXTUREDRECT2D_HPP

#include <GraphicsController.hpp>
#include <Rect2D.hpp>
#include <SDL2/SDL.h>
#include <functional>
#include <glad/glad.h>
#include <iostream>
#include <utility>
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

    union UniformVariableValue {
        GLfloat uniform1f;
    };
    // Currently only supports 1 value uniforms
    struct UniformVariable {
        std::string name;
        UniformVariableValue value;
        std::string uniformFunc;
    };

    void AddUniformVariable(std::string variableName,
                            UniformVariableValue initialValue,
                            std::string uniformFunction);

    void SetUniformVariable(std::string variableName,
                            UniformVariableValue value);

  private:
    GLenum m_textureFormat;
    GLenum m_internalTextureFormat;
    bool m_enableBlend;

    std::unordered_map<std::string, UniformVariable> m_uniformVariables;
};

#endif
