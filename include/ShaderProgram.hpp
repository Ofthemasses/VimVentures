#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glad/glad.h>

class ShaderProgram {
  private:
    friend class ShaderProgramBuilder;
    struct _passKey_t {
        _passKey_t() = default;
    };
    const GLuint m_program_id = 0;

  public:
    ~ShaderProgram();
    [[nodiscard]] GLuint GetProgramId() const;
    ShaderProgram(_passKey_t, GLuint program_id);
};
#endif
