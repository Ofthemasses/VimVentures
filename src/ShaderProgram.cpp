#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(_passKey_t _passKey, GLuint program_id) : m_program_id(program_id){};

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(m_program_id);
}

GLuint ShaderProgram::GetProgramId() const{
    return m_program_id;
}
