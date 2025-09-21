# "cmake-lists": rst, "author": alessandromanzini
# top-level cmake file, includes the lib and test subdirectories.
#
cmake_minimum_required( VERSION 3.26 )
project( "rst"
    VERSION 0.1.0
    DESCRIPTION "Rhaster Engine - A modern C++ game engine with ECS architecture"
    LANGUAGES CXX
)

# ========================================
# Global Configuration
# ========================================

# C++ Standard Configuration
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
message( STATUS "C++${CMAKE_CXX_STANDARD} support has been enabled by default." )

# Build Type Configuration
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE )
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" )
endif()
message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

# ========================================
# Compiler-Specific Settings
# ========================================

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
    message( STATUS "Using Clang compiler configuration" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    message( STATUS "Using GCC compiler configuration" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
    message( STATUS "Using MSVC compiler configuration" )
    # Enable parallel compilation on MSVC
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
endif()

# ========================================
# Output Directories
# ========================================

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# Set output directories for all build configurations
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
endforeach()

# ========================================
# CMake Module Path
# ========================================

# Append cmake module path, making sure cmake finds our custom modules (.cmake)
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# ========================================
# Project Options
# ========================================

option( RST_BUILD_TESTS "Build test executable" ON )
option( RST_BUILD_EXAMPLES "Build example applications" OFF )
option( RST_BUILD_DOCUMENTATION "Build documentation" OFF )
option( RST_ENABLE_PROFILING "Enable profiling support" OFF )

# ========================================
# Add Subdirectories
# ========================================

# Core library
add_subdirectory( "lib" )

# Test executable
if( RST_BUILD_TESTS )
    add_subdirectory( "test" )
endif()

# Examples (future)
if( RST_BUILD_EXAMPLES AND EXISTS "${CMAKE_SOURCE_DIR}/examples" )
    add_subdirectory( "examples" )
endif()

# Documentation (future)
if( RST_BUILD_DOCUMENTATION AND EXISTS "${CMAKE_SOURCE_DIR}/docs" )
    add_subdirectory( "docs" )
endif()
