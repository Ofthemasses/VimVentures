#include <GraphicsController.hpp>

std::optional<Error> GraphicsController::CheckGLObjectStatus(
    GLuint object, GLenum parameter,
    const std::function<void(GLuint, GLenum, GLint *)> &getiv,
    const std::function<void(GLuint, GLsizei, GLsizei *, GLchar *)>
        &getInfoLog) {
    GLint status;
    getiv(object, parameter, &status);

    if (status == GL_TRUE) {
        return std::nullopt;
    }

    GLint infoLen = 0;
    char *infoLog = nullptr;

    getiv(object, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen - 1 > 0) {
        infoLog = (GLchar *)malloc(sizeof(GLchar) * infoLen);

        GLsizei _infoLen;
        getInfoLog(object, infoLen, &_infoLen, infoLog);
    }

    return Error(infoLen, infoLog);
}
