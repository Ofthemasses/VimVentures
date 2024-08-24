def Settings(**kwargs):
    return {
        'flags': [
            '-x',
            'c++',
            '-Wall',
            '-I','./include',
            '-I', '/usr/include/SDL2',
            '-L', '/path/to/libs',
            '-lSDL2'],
    }

