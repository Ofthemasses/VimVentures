#ifndef ERROR_HPP
#define ERROR_HPP

#include<string>

struct Error{
    const int m_infoLogLen;
    const char* m_infoLog;
    Error(int infoLogLen, const char* infoLog);
    std::string toString();
};

#endif
