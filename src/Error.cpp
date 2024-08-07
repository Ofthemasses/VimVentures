#include <Error.hpp>

Error::Error(int infoLogLen, const char *infoLog)
    : m_infoLogLen(infoLogLen), m_infoLog(infoLog){};

std::string Error::toString() const {
    return std::string(m_infoLog, m_infoLogLen);
}
