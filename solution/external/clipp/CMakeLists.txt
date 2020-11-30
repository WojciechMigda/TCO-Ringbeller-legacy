# ./external/clipp/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.clipp )
set( VERSION 1.0 )
set( DESCRIPTION "Clipp header-only lib." )

# Provide target for "clipp" library.
add_library( clipp UNKNOWN
    IMPORTED GLOBAL )
# Store include search-path containing headers
# of "json".
set_target_properties( clipp PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
# Require at least compiling with C++14.
#set_target_properties( clipp PROPERTIES
#    MY_COMPILE_FEATURES "-std=c++1z" )
