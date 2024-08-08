#include <GraphicsController.hpp>
#include <iostream>

void GraphicsController::debugMessage(GLenum source, GLenum type,
                                      GLuint debug_id, GLenum severity,
                                      GLsizei length, const GLchar *message,
                                      const void *userParam) {
    std::cout << severity << std::endl;
    std::string message_str(message, length);
    std::cout << message_str << '\n';
}

void GraphicsController::enableDebug() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GraphicsController::debugMessage, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
}

std::optional<Error> GraphicsController::initGL(SDL_Window *sdlWindow) {
    SDL_GLContext s_glContext = SDL_GL_CreateContext(sdlWindow);

    if (s_glContext == nullptr) {
        std::string errorMsg{"OpenGL context not available: ", SDL_GetError()};
        return Error(errorMsg.size(), errorMsg.c_str());
    }

    // Initialize GL
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
        std::cerr << "Glad Not Initialised: " << std::endl;
        exit(1);
    }

    return std::nullopt;
}

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
