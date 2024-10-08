cmake_minimum_required(VERSION 3.10)
project(VimVentures)

file(GLOB SOURCES "src/*.cpp")
add_executable(VimVentures ${SOURCES} src/glad.c)

target_include_directories(VimVentures PRIVATE include)

find_package(SDL2 REQUIRED)
target_link_libraries(VimVentures PRIVATE SDL2)
target_link_libraries(VimVentures PRIVATE SDL2_ttf)
target_link_libraries(VimVentures PRIVATE SDL2_image)
target_link_libraries(VimVentures PRIVATE util X11 pthread dl)

set_target_properties(VimVentures PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

configure_file(vim-ventures-autoload.vim ${CMAKE_BINARY_DIR}/bin/vim-ventures-
autoload.vim COPYONLY)

add_custom_target(
	tidy
    COMMAND clang-tidy src/*.cpp -- -I/usr/include/SDL2 -I/user/include/SDL2_ttf -I/user/include/SDL2_image -Iinclude/ -std=c++17
)

add_custom_target(
	format
	COMMAND find ./src ./include -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i
)

