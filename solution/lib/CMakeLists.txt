set( CMAKE_CXX_STANDARD 17 )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0067 NEW) # to preserve CXX standard for check_include_file_cxx
include(CheckIncludeFileCXX)
check_include_file_cxx("optional" HAS_CXX_OPTIONAL)
if(NOT HAS_CXX_OPTIONAL)
    message(FATAL_ERROR "C++ <optional> header is required but is missing")
endif()
check_include_file_cxx("variant" HAS_CXX_VARIANT)
if(NOT HAS_CXX_VARIANT)
    message(FATAL_ERROR "C++ <variant> header is required but is missing")
endif()

################################################################################

set(SOURCES
    src/poc.cpp
)

add_library( ringbeller_ObjLib OBJECT src/dummy.cpp )
target_sources( ringbeller_ObjLib PRIVATE ${SOURCES} )

target_include_directories(ringbeller_ObjLib
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ringbeller_ObjLib PRIVATE cxx_rvalue_references)

target_include_directories( ringbeller_ObjLib PRIVATE ${Boost_INCLUDE_DIRS})

target_compile_features( ringbeller_ObjLib
    PUBLIC $<TARGET_PROPERTY:Boost::system,INTERFACE_COMPILE_FEATURES> )

################################################################################

add_library( ringbeller SHARED src/dummy.cpp )
target_sources( ringbeller PRIVATE
  $<TARGET_OBJECTS:ringbeller_ObjLib>
)

target_include_directories( ringbeller
    PUBLIC
        $<TARGET_PROPERTY:ringbeller_ObjLib,INTERFACE_INCLUDE_DIRECTORIES> )
target_compile_features( ringbeller
    PUBLIC $<TARGET_PROPERTY:ringbeller_ObjLib,INTERFACE_COMPILE_FEATURES> )
target_link_libraries( ringbeller PUBLIC Boost::system )

################################################################################

add_library( ringbeller_static src/dummy.cpp )
target_sources( ringbeller_static PRIVATE
  $<TARGET_OBJECTS:ringbeller_ObjLib>
)

target_include_directories( ringbeller_static
    PUBLIC
        $<TARGET_PROPERTY:ringbeller_ObjLib,INTERFACE_INCLUDE_DIRECTORIES> )
target_compile_features( ringbeller_static
    PUBLIC $<TARGET_PROPERTY:ringbeller_ObjLib,INTERFACE_COMPILE_FEATURES> )
target_link_libraries( ringbeller_static PUBLIC Boost::system )

################################################################################

# Create target 'install' for installing libraries.
install(TARGETS ringbeller ringbeller_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/ringbeller)
