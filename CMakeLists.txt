cmake_minimum_required(VERSION 3.16)
project(CSX VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.70.0 REQUIRED COMPONENTS system thread)

# Only fetch rpclib - don't fetch msgpack-c separately
include(FetchContent)
FetchContent_Declare(
  rpclib
  GIT_REPOSITORY https://github.com/rpclib/rpclib
  GIT_TAG        v2.2.1
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(rpclib)

# Include directories for headers
include_directories(
    ${PROJECT_SOURCE_DIR}/src/headers
    ${PROJECT_SOURCE_DIR}/src/service
    ${Boost_INCLUDE_DIRS}  # Add Boost include path
    ${PROTO_SRC_DIR}
    ${rpclib_SOURCE_DIR}/include  # Add rpclib include path
)

# Source files
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
)

# Add main.cpp from root directory
set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${SRC_FILES}
    ${PROTO_GENERATED_SOURCES}
)

# Header files
file(GLOB_RECURSE HEADERS
    "${PROJECT_SOURCE_DIR}/src/headers/*.h"
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    rpc
    ${Boost_LIBRARIES}
)

# Test executable for MarketClient
add_executable(MarketClient ${PROJECT_SOURCE_DIR}/src/test/MarketClientTest.cc)
target_link_libraries(MarketClient PRIVATE 
    rpc
    ${Boost_LIBRARIES}  # Link Boost libraries 
)

# Add custom test target
add_custom_target(test
    DEPENDS MarketClient
    COMMAND MarketClient
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running MarketClient test..."
    )

# Add custom test target
add_custom_target(main
    DEPENDS ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running main..."
    )