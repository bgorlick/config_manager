cmake_minimum_required(VERSION 3.10)

project(ConfigManager)

# Check if C++20 is available, otherwise fall back to C++17
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Create the library (header-only, so no source files)
add_library(config_manager INTERFACE)
target_include_directories(config_manager INTERFACE include)

# Find and link required libraries
find_library(YAMLCPP_LIB yaml-cpp)
find_library(JSONCPP_LIB jsoncpp)

target_link_libraries(config_manager INTERFACE ${YAMLCPP_LIB} ${JSONCPP_LIB})

# Create the test executable
add_executable(test_configuration tests/test_configuration.cpp)

# Link the test executable with the config_manager library
target_link_libraries(test_configuration config_manager)
