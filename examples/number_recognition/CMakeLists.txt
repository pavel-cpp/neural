cmake_minimum_required(VERSION 3.23)
project(NeuralNetwork)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_PREFIX_PATH path/to/SFML/lib/cmake path/to/Neural/share/cmake) # change here
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build) # change here

set(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(SFML COMPONENTS window graphics system CONFIG REQUIRED)

include_directories(path/to/Neural/include/) # change here

find_package(Neural REQUIRED)

add_executable(
        NeuralNetwork
        main.cpp
)

target_link_libraries(NeuralNetwork PUBLIC Neural_Perceptron Neural_Functions)

target_link_libraries(NeuralNetwork PRIVATE sfml-graphics sfml-window sfml-system)

