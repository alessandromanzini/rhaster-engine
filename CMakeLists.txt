# "cmake-lists": master, "author": alessandromanzini
# project configuration specifics.
#
cmake_minimum_required( VERSION 3.26 )
project( "master" )

# set the C++ standard to C++20 and ensure it is required
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
message( STATUS "c++20 support has been enabled by default." )
# target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
    message( STATUS "using clang config." )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library" )
endif()

# append cmake module path, making sure cmake finds our custom modules (.cmake)
set( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake" )

# include the lib subdirectory
add_subdirectory( "lib" )
