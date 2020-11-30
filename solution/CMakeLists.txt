cmake_minimum_required( VERSION 3.5 )

project( Ringbeller )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

find_package( Boost REQUIRED COMPONENTS system )

# Make external libraries globally available.
add_subdirectory( external )

add_subdirectory( lib )

add_subdirectory( app )
