# "cmake-lists": rst, "author": alessandromanzini
# top-level cmake file, includes the lib and test subdirectories.
#
cmake_minimum_required( VERSION 3.26 )
project( "rst" )

# set the C++ standard to C++20 and ensure it is required
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
message( STATUS "C++${CMAKE_CXX_STANDARD} support has been enabled by default." )
# target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
    message( STATUS "using clang config." )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library" )
endif()

# append cmake module path, making sure cmake finds our custom modules (.cmake)
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# include the lib subdirectory
add_subdirectory( "lib" )
add_subdirectory( "test" )
