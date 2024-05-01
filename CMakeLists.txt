cmake_minimum_required(VERSION 3.10)
project(energy_monitor VERSION 1.0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})





message(STATUS "Source files: ${CMAKE_BINARY_DIR}")
message(STATUS "Source files: ${CMAKE_SOURCE_DIR}")


# Include FetchContent module
include(FetchContent)

# Fetch and build yaml-cpp
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()




file(GLOB_RECURSE SOURCES "src/*.cpp")
message(STATUS "Source files: ${SOURCES}")

# Add header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for yaml-cpp
target_include_directories(${PROJECT_NAME} PUBLIC ${yaml-cpp_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp)

# TO CMAKE THE PROJECT AND MAKE it suitable for compiling run 
#   cmake .

# TO build / compile the project run 
# make

# RUN 
# ./energy_monitor to run the binary

# for visual studio integration run  
#   cmake -G "Visual Studio 16 2022" --config Release .






