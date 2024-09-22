cmake_minimum_required (VERSION 3.16)

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

add_subdirectory(external)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package (SQLite3 REQUIRED)

add_library(protolib proto/score.proto)
target_link_libraries(protolib gRPC::grpc++)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(SRC_CORE
    src/Application.cpp
    src/Logger.cpp
    src/Window.cpp
    src/WindowServer.cpp
    src/WindowClient.cpp
    src/ServiceServer.cpp
    src/ServiceClient.cpp
    src/Tile.cpp
)
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(Core STATIC ${SRC_CORE} ${IMGUI_SRC} ${CMAKE_CURRENT_BINARY_DIR}/proto/score.grpc.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/proto/score.pb.cc)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    src
	external/include
	external/spdlog/include
	external/glfw/include
	external/imgui
	external/imgui/backends
)
target_link_libraries(Core PUBLIC spdlog glfw gRPC::grpc++ SQLite::SQLite3)

add_executable(Client src/ApplicationClient.cpp)
target_include_directories(Client PUBLIC src)
target_link_libraries(Client PUBLIC Core)

add_executable(Server src/ApplicationServer.cpp)
target_include_directories(Server PUBLIC src)
target_link_libraries(Server PUBLIC Core)