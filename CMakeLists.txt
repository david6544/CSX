cmake_minimum_required(VERSION 3.10)
project(CSX VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for headers
include_directories(${PROJECT_SOURCE_DIR}/src/headers)

# Source files - explicitly add main.cpp from root directory
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/impl/*.cpp"
)

# Add main.cpp from root directory
set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${SRC_FILES}
)

# Header files
file(GLOB_RECURSE HEADERS
    "${PROJECT_SOURCE_DIR}/src/headers/*.h"
)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})