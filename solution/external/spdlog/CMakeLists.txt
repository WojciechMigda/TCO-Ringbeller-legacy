# ./external/spdlog/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.spdlog )
set( VERSION 1.0 )
set( DESCRIPTION "spdlog header-only lib." )

# Provide target for "spdlog" library.
add_library( spdlog UNKNOWN
    IMPORTED GLOBAL )
# Store include search-path containing headers
# of "spdlog".
set_target_properties( spdlog PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
