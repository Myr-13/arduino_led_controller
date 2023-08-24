cmake_minimum_required(VERSION 3.20)
project(led_controller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

message("[*] Downloading SFML")
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
message("[*] Downloading SFML - done")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "imgui/*.cpp" "imgui/*.h")

add_executable(led_controller ${SOURCES})
target_include_directories(led_controller PUBLIC src imgui)
target_link_libraries(led_controller PRIVATE sfml-graphics opengl32.lib)
target_compile_features(led_controller PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET led_controller POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:led_controller> $<TARGET_FILE_DIR:led_controller> COMMAND_EXPAND_LISTS)
endif()
