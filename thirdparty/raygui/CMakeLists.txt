
project(raygui LANGUAGES C)

include_directories("src" "../raylib/src")

add_library(${PROJECT_NAME} STATIC "src/raygui.c")
add_dependencies(${PROJECT_NAME} "raylib")
