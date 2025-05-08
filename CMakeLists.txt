cmake_minimum_required(VERSION 3.28)

project(text_speed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_PATH "C:/Libraries/CXX/SDL2-2.30.11")
set(SDL2_TTF_PATH "C:/Libraries/CXX/SDL2_ttf-2.24.0")

include_directories(
    "${SDL2_PATH}/include",
    "${SDL2_TTF_PATH}/include")

link_directories(
    "${SDL2_PATH}/lib/x64",
    "${SDL2_TTF_PATH}/lib/x64")

add_executable(text_speed main.cpp)

target_link_libraries(text_speed
    PRIVATE
        -lSDL2 -lSDL2main -lopengl32 -lSDL2_ttf
)
