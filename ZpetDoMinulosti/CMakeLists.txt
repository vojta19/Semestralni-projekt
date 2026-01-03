cmake_minimum_required(VERSION 3.10)
project(ZpetDoMinulosti)

find_package(CURL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Pro Linux/DevContainer: Stáhneme SFML automaticky ---
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
FetchContent_MakeAvailable(SFML)

# --- Zbytek nastavení ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(ZpetDoMinulosti ${SOURCES})

target_include_directories(ZpetDoMinulosti PRIVATE src)

# Linkování (pro SFML 3.0)
target_link_libraries(ZpetDoMinulosti PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    SFML::Network
    CURL::libcurl
)
