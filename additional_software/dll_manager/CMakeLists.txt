CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
project(NeuralDLLManager)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-static")

add_executable(${PROJECT_NAME} dll_manager.cpp)