cmake_minimum_required(VERSION 3.21)
project(JourneyToTheClouds)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

if(DEFINED ENV{SFML_DIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{SFML_DIR}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{SFML_DIR}/lib/cmake/SFML")
else()
    list(APPEND CMAKE_PREFIX_PATH "C:/SFML-3.0.2")
    list(APPEND CMAKE_PREFIX_PATH "C:/SFML-3.0.2/lib/cmake/SFML")
endif()

message(STATUS "Looking for SFML...")
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

if(MSVC)
    add_compile_options("/Zc:__cplusplus")
endif()

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
target_link_libraries(JourneyToTheClouds PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

if(WIN32)
    add_custom_command(TARGET JourneyToTheClouds POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib"
        "$<TARGET_FILE_DIR:JourneyToTheClouds>"
    )
endif()

add_custom_command(TARGET JourneyToTheClouds POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:JourneyToTheClouds>/assets"
)
