cmake_minimum_required(VERSION 3.31)
project(sc2fix)

set(CMAKE_CXX_STANDARD 20)

add_executable(sc2fix 
        src/main.cpp
        src/Config.cpp
        src/Config.h
        src/DirectoryWatcher.cpp
        src/DirectoryWatcher.h
        src/Logger.h
        src/ReplayFile.cpp
        src/ReplayFile.h
        src/ReplayPatcher.cpp
        src/ReplayPatcher.h
        src/Sc2Version.cpp
        src/Sc2Version.h
)

