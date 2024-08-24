#include "ShaderProgram.hpp"

/**
 * Encapsulation of an OpenGL program.
 *
 * @param passkey An instance of the _passKey_t struct. This restricts
 * the constructor usage.
 * @param program_id The OpenGL shader program ID.
 * @details The passkey is used to restrict instantiation of ShaderProgram
 * objects Only classes that have access to the _passKey_t struct can create
 * ShaderProgram objects.
 */
ShaderProgram::ShaderProgram(_passKey_t _passKey, GLuint program_id)
    : m_program_id(program_id){};

/**
 * Cleans up the program on destruction.
 */
ShaderProgram::~ShaderProgram() { glDeleteProgram(m_program_id); }

/**
 * Get the programID.
 *
 * @return OpenGL Program ID as GLuint.
 */
GLuint ShaderProgram::GetProgramId() const { return m_program_id; }
