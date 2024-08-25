cmake_minimum_required (VERSION 3.20)

project ("2048")

option(DEBUG "Enable debug mode" OFF)
if(DEBUG)
    add_definitions(-DDEBUG)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(external/glfw)