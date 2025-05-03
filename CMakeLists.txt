cmake_minimum_required(VERSION 3.14)
project(bounce LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Use pkg-config instead of find_package
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-graphics sfml-window sfml-system)

add_executable(bounce main.cpp)
target_link_libraries(bounce PRIVATE ${SFML_LIBRARIES})
target_include_directories(bounce PRIVATE ${SFML_INCLUDE_DIRS})
target_compile_options(bounce PRIVATE ${SFML_CFLAGS_OTHER})
target_link_directories(bounce PRIVATE ${SFML_LIBRARY_DIRS})