# This is first version of NeuralLib please dont try to compile this
# use including by find_package()
# But if you want to try it, you can read the code below

cmake_minimum_required(VERSION 3.12)
project(Neural VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY lib)

# Definition of the main goal
add_library(${PROJECT_NAME}_Functions SHARED
        src/Activation/Functions.cpp
        additional_software/dll_manager/dll_manager.cpp
)

add_library(${PROJECT_NAME}_Perceptron SHARED
        src/Perceptron/Perceptron.cpp
        src/Perceptron/Layer.cpp
        )

# TODO: Delete this later
#add_library(${PROJECT_NAME} SHARED
#        src/Perceptron/Perceptron.cpp src/Perceptron/Layer.cpp
#        )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)  # Adding an include folder

# Create an installation package
install(TARGETS
        ${PROJECT_NAME}_Perceptron
        ${PROJECT_NAME}_Functions
        EXPORT ${PROJECT_NAME}Config
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(DIRECTORY include/ DESTINATION include)

install(EXPORT ${PROJECT_NAME}Config
        DESTINATION share/${PROJECT_NAME}/cmake
        )

export(TARGETS
        ${PROJECT_NAME}_Perceptron
        ${PROJECT_NAME}_Functions
        FILE ${PROJECT_NAME}Config.cmake
        )