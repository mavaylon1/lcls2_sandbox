cmake_minimum_required(VERSION 3.10 FATAL_ERROR) # set minimum cmake

project(xtcdata) # set the project name

set(CMAKE_CXX_STANDARD 11) # set just creates a variable
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib") # CMAKE_INSTALL_PREFIX is a cmake variable that defines the base directory where a cmake based project will be installed on the system.
# by default it is '/usr/local' on unix systems. Here we are saying set it to /usr/local/lib
# common locations where packages can be installed /usr/bin: Executable files || /usr/lib or /usr/lib64: Libraries || /usr/include: Header files
# /usr/share: Shared data files
# /usr/local/bin, /usr/local/lib, etc.: Similar to above, but for locally installed software (not managed by the package manager)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# cmake commands to possibly use in the future

# skipped this 
#file(GLOB_RECURSE
#     ALL_CXX_SOURCE_FILES
#     *.cc *.hh
#)

# skipped this 
# Adding clang-format target if executable is found
#find_program(CLANG_FORMAT "clang-format")
#if(CLANG_FORMAT)
#  add_custom_target(
#    clang-format
#    COMMAND ${CLANG_FORMAT}
#    -i
#    -style=file
#    ${ALL_CXX_SOURCE_FILES}
#)
#endif()

# cmake builds the source directory; however, it will not natively build the subfolders. This command adds the subdirectory to the build process. 
# You also need to make sure that there is a CMakeLists.txt in that subdirectory. It can be empty or it can have more add_subdirectory calls.
add_subdirectory(xtcdata)

include(CMakePackageConfigHelpers) # This is includes a set of CMake functions that help package generation. 
# CMakePackageConfigHelpers is where install and configure_package_config_file are from (and other functions). 
# This is essentially import from python

# When you run install, it creates a .cmake file. This file is typically named <exportsetname>Targets.cmake. We define the export set above with EXPORT.
# What is an export set? It is a collection of targets that are intended to be used by other CMake projects. Other cmake projects can use "find_package" to locate and import the export set, allowing the "other project" to use the targets.
install(EXPORT xtcdataTargets
    FILE xtcdataTargets.cmake
    NAMESPACE xtcdata::
    DESTINATION lib/cmake/xtcdata
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/xtcdataConfig.cmake.in"
    ${CMAKE_CURRENT_BINARY_DIR}/xtcdataConfig.cmake
    INSTALL_DESTINATION lib/cmake/xtcdata
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/xtcdataConfig.cmake
    DESTINATION lib/cmake/xtcdata
)
