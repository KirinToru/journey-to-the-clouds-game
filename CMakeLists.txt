cmake_minimum_required(VERSION 3.21)
project(JourneyToTheClouds)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

if(DEFINED ENV{SFML_DIR})
    set(SFML_DIR "$ENV{SFML_DIR}")
else()
    set(SFML_DIR "C:/SFML-3.0.2")
endif()

message(STATUS "Using SFML from: ${SFML_DIR}")

if(MSVC)
    add_compile_options("/Zc:__cplusplus")
endif()

# Fetch SFML based on defined mode
include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)
set(SOURCES
    "src/Game/main.cpp"
    "src/Game/Game.cpp"
    "src/Game/Entities/Player.cpp"
    "src/Game/World/Map.cpp"
    "src/Game/States/MenuState.cpp"
    "src/Game/States/GameState.cpp"
    "src/Game/States/PauseState.cpp"
    "src/Engine/GUI/Button.cpp"
)
add_executable(JourneyToTheClouds ${SOURCES})

target_include_directories(JourneyToTheClouds PRIVATE include)
target_link_libraries(JourneyToTheClouds
    debug sfml-graphics-d
    optimized sfml-graphics
    debug sfml-window-d
    optimized sfml-window
    debug sfml-system-d
    optimized sfml-system
    debug sfml-audio-d
    optimized sfml-audio
)
add_custom_command(TARGET JourneyToTheClouds POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/lib"
    "$<TARGET_FILE_DIR:JourneyToTheClouds>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:JourneyToTheClouds>/assets")
