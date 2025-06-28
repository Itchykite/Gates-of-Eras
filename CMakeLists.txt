cmake_minimum_required(VERSION 3.16)
project(Gates_of_Eras LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Werror
        -fcolor-diagnostics
        -fansi-escape-codes
        -O0
        -g
        -lX11
)
endif()

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp
                                    src/Renderer/*.cpp
                                    src/Window/*.cpp
                                    src/Ext/*.cpp
                                    src/Config/*.cpp
                                    src/Tilemap/*.cpp
                                )

add_executable(Gates_of_Eras ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

target_include_directories(Gates_of_Eras PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(Gates_of_Eras PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
target_include_directories(Gates_of_Eras PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Gates_of_Eras PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(Gates_of_Eras PRIVATE ${SDL2_TTF_LIBRARIES})
target_link_libraries(Gates_of_Eras PRIVATE ${SDL2_IMAGE_LIBRARIES})

target_include_directories(Gates_of_Eras PRIVATE ${CMAKE_SOURCE_DIR}/src)
