#include "TexturedRect2D.hpp"

/**
 * An extension of Rect2D, representing a 2D quad with a texture.
 *
 * @param xPos X position
 * @param yPos Y position
 * @param width Width of the quad
 * @param height Height of the quad
 */
TexturedRect2D::TexturedRect2D(GLfloat xPos, GLfloat yPos, GLfloat width,
                               GLfloat height)
    : Rect2D(xPos, yPos, width, height) {

    m_textureFormat = GL_BGRA;
    m_internalTextureFormat = GL_RGBA;
}

/**
 * Deletes the texture on destruction.
 */
TexturedRect2D::~TexturedRect2D() { glDeleteTextures(1, &m_texture); }

/**
 * Updates the vertex data. Adds two more data points
 * to the strides defining the texture coordinates.
 */
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

/**
 * Sets the texture with raw pixel data. Unless forced or the new texture has
 * a different width and/or height will sub-image the texture rather than
 * generate a new one. This helps for texture streaming such as in VimEmulator.
 *
 * @param data Pointer to pixel data
 * @param width Width of the texture in pixels
 * @param height Height of the texture in pixels
 * @param force If set to true forces the texture to be re-generated
 */
void TexturedRect2D::SetTexture(void *data, GLuint width, GLuint height,
                                bool force) {
    if (width == m_texture_width && height == m_texture_height && !force) {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, m_textureFormat,
                        GL_UNSIGNED_BYTE, data);
        return;
    }

    if (glIsTexture(m_texture)) {
        glDeleteTextures(1, &m_texture);
    }

    m_texture_width = width;
    m_texture_height = height;

    if (m_enableBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, m_internalTextureFormat, width, height, 0,
                 m_textureFormat, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    UpdateGL();
}

/**
 * Sets the format for the pixel data given to SetTexture.
 */
void TexturedRect2D::SetTextureFormat(GLenum format) {
    m_textureFormat = format;
}

/**
 * Sets the internal texture format for the glTexImage.
 */
void TexturedRect2D::SetInternalTextureFormat(GLenum format) {
    m_internalTextureFormat = format;
}

/**
 * Enabled texture blending.
 */
void TexturedRect2D::EnableTextureBlend() { m_enableBlend = true; };

/**
 * Disables texture blending.
 */
void TexturedRect2D::DisableTextureBlend() { m_enableBlend = false; };

/**
 * Renders the Textured Quad. Currently uses the sp_cathode shader.
 *
 * @todo Gives texture rects a default shader program and method to
 * select others.
 */
void TexturedRect2D::Render() {
    glUseProgram(GraphicsController::s_shaderPrograms.at(m_shaderProgram)
                     ->GetProgramId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    GLint programID = GraphicsController::s_shaderPrograms.at(m_shaderProgram)
                          ->GetProgramId();
    GLint textureLocation = glGetUniformLocation(programID, "u_Texture");
    glUniform1i(textureLocation, 0);
    if (!m_uniformVariables.empty()) {
        for (auto &uniformVariable : m_uniformVariables) {
            GLint location = glGetUniformLocation(
                programID, uniformVariable.second.name.c_str());
            // This is very hardcoded TODO allow for automatic union
            // determination
            if (uniformVariable.second.uniformFunc == "glUniform1f") {
                glUniform1f(location, uniformVariable.second.value.uniform1f);
            }
        }
    }

    Rect2D::Render();
}

/**
 * Updates the vertex arrays in the OpenGL FSM.
 * Uses strides to get texture data as well as coordinate data.
 */
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

void TexturedRect2D::AddUniformVariable(std::string variableName,
                                        UniformVariableValue initialValue,
                                        std::string uniformFunction) {
    UniformVariable uniformVariable = {variableName, initialValue,
                                       uniformFunction};
    m_uniformVariables.emplace(variableName, uniformVariable);
}

void TexturedRect2D::SetUniformVariable(std::string variableName,
                                        UniformVariableValue value) {
    m_uniformVariables.at(variableName).value = value;
}
