cmake_minimum_required (VERSION 3.20)

project ("2048")

option(DEBUG "Enable debug mode" OFF)
if(DEBUG)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(external/glfw)