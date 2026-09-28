cmake_minimum_required(VERSION 3.20)
project(CorePulse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Includem folderul 'core' ca subproiect
add_subdirectory(core)

# Executabilul principal care va folosi librăria core
add_executable(CorePulseApp main.cpp)
target_link_libraries(CorePulseApp PRIVATE core)
