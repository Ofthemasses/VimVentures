#include <ShaderProgramBuilder.hpp>

#include <fstream>
#include <optional>
#include <GraphicsController.hpp>
#include <iostream>

ShaderProgramBuilder::~ShaderProgramBuilder(){
    // Clean up shaders
    std::for_each(m_shaders.begin(), m_shaders.end(), glDeleteShader);
}

ShaderProgramBuilder& ShaderProgramBuilder::LoadShaderFile(GLenum type, std::string filePath){
    if (m_error.has_value()){
        return *this;
    }

    std::string shader_str;
    std::string line;
    std::ifstream fileStream(filePath.c_str());

    if (fileStream.is_open()) {
        while (std::getline(fileStream, line)){
            shader_str += line + "\n";
        }
        fileStream.close();
     }

    return LoadShader(type, shader_str);
}

ShaderProgramBuilder& ShaderProgramBuilder::LoadShader(GLenum type, std::string shaderCode){
    if (m_error.has_value()){ return *this; }

    GLuint shaderObject = glCreateShader(type);
    const char *src = shaderCode.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    std::optional<Error> compileResult = GraphicsController::CheckGLObjectStatus(
            shaderObject,
            GL_COMPILE_STATUS,
            glGetShaderiv,
            glGetShaderInfoLog
            );

    if (compileResult.has_value()){
        m_error.emplace(compileResult.value());
        glDeleteShader(shaderObject);
        return *this;
    }

    m_shaders.push_back(shaderObject);
    return *this;
}

std::variant<Error, std::unique_ptr<ShaderProgram>> ShaderProgramBuilder::GenerateShaderProgram(){
    if (m_error.has_value()){
        return m_error.value();
    }

    GLuint programObject = glCreateProgram();

    for (GLuint &shader : m_shaders){
        std::cout << shader << std::endl;
        glAttachShader(programObject, shader);
    }
    glLinkProgram(programObject);

    std::optional<Error> linkResult = GraphicsController::CheckGLObjectStatus(
            programObject,
            GL_LINK_STATUS,
            glGetProgramiv,
            glGetProgramInfoLog
            );

    if (linkResult.has_value()){
        m_error.emplace(linkResult.value());
        glDeleteProgram(programObject);
        return m_error.value();
    }

    glValidateProgram(programObject);

    std::optional<Error> validateResult = GraphicsController::CheckGLObjectStatus(
            programObject,
            GL_VALIDATE_STATUS,
            glGetProgramiv,
            glGetProgramInfoLog
            );

    if (validateResult.has_value()){
        m_error.emplace(validateResult.value());
        glDeleteProgram(programObject);
        return m_error.value();
    }

    // Clean up shaders
    std::for_each(m_shaders.begin(), m_shaders.end(), glDeleteShader);
    m_shaders.clear();
    return std::make_unique<ShaderProgram>(ShaderProgram::_passKey_t{}, programObject);
}
