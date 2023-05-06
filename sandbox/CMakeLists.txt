cmake_minimum_required(VERSION 3.3)
include("../CMakeHelpers.cmake")

project("sandbox" LANGUAGES C)

list_sources(SANDBOX_SOURCES "src")

add_executable(${PROJECT_NAME} ${SANDBOX_SOURCES})
add_dependencies(${PROJECT_NAME} "pong")
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BUILD_DIRECTORY}/${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BUILD_DIRECTORY}/${PROJECT_NAME}"
)

include_directories("src")
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES} "lib-pong")
