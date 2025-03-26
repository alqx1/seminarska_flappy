cmake_minimum_required(VERSION 3.28)
project(collision LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#[[
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG 35b1148efb839381b84de9290d9caf0b66ad7d03
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG 82dc2033e51b8323857c3ae1cf1f458b3a933c35
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO OFF)
FetchContent_MakeAvailable(imgui-sfml)
]]

FILE(GLOB_RECURSE CPPSOURCES ./src/*.cpp)
add_executable(flappy ${CPPSOURCES})
target_link_libraries(flappy PRIVATE sfml-graphics sfml-audio)
# target_link_libraries(flappy PRIVATE ImGui-SFML::ImGui-SFML)
target_compile_features(flappy PRIVATE cxx_std_17)

add_custom_target(copy_audio
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/audio ${CMAKE_CURRENT_BINARY_DIR}/bin/audio
)
add_custom_target(copy_sprites
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/sprites ${CMAKE_CURRENT_BINARY_DIR}/bin/sprites
)
add_dependencies(flappy copy_audio copy_sprites)

if(WIN32)
    add_custom_command(
        TARGET flappy
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:flappy>
        VERBATIM)
endif()
