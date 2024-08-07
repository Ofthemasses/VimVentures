#include<Error.hpp>
#include <stdlib.h>
Error::Error(int infoLogLen, const char* infoLog) : m_infoLogLen(infoLogLen), m_infoLog(infoLog){};

std::string Error::toString(){
    return std::string(m_infoLog, m_infoLogLen);
}
