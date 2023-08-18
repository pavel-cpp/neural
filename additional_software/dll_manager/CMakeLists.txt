CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
project(NeuralDLLManager)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../lib/)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-static")

add_executable(
        ${PROJECT_NAME}
        dll_manager.cpp
        CLI/src/ProgressBar.cpp
        CLI/include/ProgressBar.hpp
        src/filesystem_interface.cpp
        include/filesystem_interface.hpp
        src/dependents_checker.cpp
        include/dependents_checker.hpp
)