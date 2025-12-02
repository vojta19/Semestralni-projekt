cmake_minimum_required(VERSION 3.10)
project(ZpetDoMinulosti)

set(CMAKE_CXX_STANDARD 17)

# Cesta k SFML
set(SFML_DIR "C:\Users\vojta\Documents\Visual Studio Code\SFML-3.0.2")

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

add_executable(ZpetDoMinulosti src/main.cpp)

target_link_libraries(ZpetDoMinulosti sfml-system sfml-window sfml-graphics sfml-audio)
