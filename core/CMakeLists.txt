cmake_minimum_required(VERSION 3.20)
project(CoreEngine LANGUAGES CXX)

# Creăm o librărie statică din codul nostru de monitorizare
add_library(core STATIC src/SystemMonitor.cpp)

# Îi spunem lui CMake unde sunt fișierele .hpp
target_include_directories(core PUBLIC include)
