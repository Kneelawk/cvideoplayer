cmake_minimum_required(VERSION 4.0)
project(cvideoplayer)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ffmpeg REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(cvideoplayer main.c)
target_link_libraries(cvideoplayer PRIVATE PkgConfig::ffmpeg glfw OpenGL::GL)
