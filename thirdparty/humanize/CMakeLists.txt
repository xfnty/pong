
project(humanize LANGUAGES C)

include_directories("src")

add_library(${PROJECT_NAME} STATIC "src/humanize.c")
