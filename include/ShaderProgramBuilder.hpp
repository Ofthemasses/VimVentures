#ifndef SHADERPROGRAMBUILDER_HPP
#define SHADERPROGRAMBUILDER_HPP

#include <Error.hpp>
#include <ShaderProgram.hpp>
#include <glad/glad.h>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

class ShaderProgramBuilder {
  public:
    ~ShaderProgramBuilder();
    ShaderProgramBuilder &LoadShaderFile(GLenum type, std::string file);
    ShaderProgramBuilder &LoadShader(GLenum type, std::string shaderCode);
    std::variant<Error, std::unique_ptr<ShaderProgram>> GenerateShaderProgram();

  private:
    std::vector<GLuint> m_shaders;
    std::optional<Error> m_error = std::nullopt;
};

#endif
