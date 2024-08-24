#include <Error.hpp>

/**
 * Error type for OpenGL errors, stored as a
 * c-string.
 *
 * @param infoLogLen Length of the string
 * @param infoLog C-string log
 */
Error::Error(int infoLogLen, const char *infoLog)
    : m_infoLogLen(infoLogLen), m_infoLog(infoLog){};

/**
 * Converts error log to std::string.
 *
 * @returns Error log as std::string
 */
std::string Error::toString() const {
    return std::string(m_infoLog, m_infoLogLen);
}
