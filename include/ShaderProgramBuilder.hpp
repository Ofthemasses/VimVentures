#ifndef SHADERPROGRAMBUILDER_HPP
#define SHADERPROGRAMBUILDER_HPP

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <memory>
#include <glad/glad.h>
#include <ShaderProgram.hpp>
#include <Error.hpp>

class ShaderProgramBuilder {
    public:
        ~ShaderProgramBuilder();
        ShaderProgramBuilder& LoadShaderFile(GLenum type, std::string file);
        ShaderProgramBuilder& LoadShader(GLenum type, std::string shaderCode);
        std::variant<Error, std::unique_ptr<ShaderProgram>> GenerateShaderProgram();

    private:
        std::vector<GLuint> m_shaders;
        std::optional<Error> m_error = std::nullopt;
};

#endif
