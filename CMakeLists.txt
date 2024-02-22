cmake_minimum_required(VERSION 3.12)

set(PROJ_NAME <PROJECT_NAME>)

# Set the compiler flags
# Wno-reorder disables the reorder warning
if (WIN32) # WIN32 is set to true when the target system is windows, including 64-bit according to the CMake docs
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-reorder")
endif()

# Set the project name
project(${PROJ_NAME})

# Give directories where header files are located
# Technically not needed as we are an executable application and main links to everything we need for us
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

# Add main.cpp to the executable
add_executable(${PROJ_NAME} main.cpp)

# Inlcude source files from the src dir
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# Use c++ 23 if supported
set_property(TARGET ${PROJ_NAME} PROPERTY CXX_STANDARD 23)
