cmake_minimum_required(VERSION 3.10)
project(energy_monitor VERSION 1.0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)




message(STATUS "Source files: ${CMAKE_BINARY_DIR}")
message(STATUS "Source files: ${CMAKE_SOURCE_DIR}")



include(FetchContent)

FetchContent_Declare(
    PDFHummus
    GIT_REPOSITORY https://github.com/galkahana/PDF-Writer.git
    GIT_TAG v4.6.2
    FIND_PACKAGE_ARGS
)
message(STATUS "downloading PDF HUMMUS")
FetchContent_MakeAvailable(PDFHummus)
message(STATUS "download finished")




file(GLOB_RECURSE SOURCES "src/*.cpp")
message(STATUS "Source files: ${SOURCES}")

# Add header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC PDFHummus::PDFWriter)
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link yaml-cpp to your project


# TO CMAKE THE PROJECT AND MAKE it suitable for compiling run 
#   cmake .

# TO build / compile the project run 
# make

# RUN 
# ./energy_monitor to run the binary

# for visual studio integration run  
#   cmake -G "Visual Studio 16 2022" --config Release .






