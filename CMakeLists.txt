cmake_minimum_required(VERSION 3.10)

set(GLTFMTLX_MAJOR_VERSION 1)
set(GLTFMTLX_MINOR_VERSION 38)
set(GLTFMTLX_BUILD_VERSION 5)
set(GLTFMTLX_LIBRARY_VERSION ${GLTFMTLX_MAJOR_VERSION}.${GLTFMTLX_MINOR_VERSION}.${GLTFMTLX_BUILD_VERSION})

project(glTF2Mtlx VERSION ${GLTFMTLX_MAJOR_VERSION}.${GLTFMTLX_MINOR_VERSION})

set(CMAKE_CXX_STANDARD 17)

# Adjust binary and library locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adjust the default installation path
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "Default install path" FORCE)

# Get MaterialX installation information
set(MATERIALX_ROOT "<MATERIALX_ROOT>" CACHE PATH "Path to the root folder of the MaterialX installation.")
message(MATERIALX_ROOT: ${MATERIALX_ROOT})
if (NOT EXISTS ${MATERIALX_ROOT})
    message(FATAL_ERROR "Could not find external MaterialX installation: ${MATERIAL_ROOT}")
endif()

set(MATERIALX_LIBRARIES CACHE "" PATH "MaterialX libraries")
set(MATERIALX_INCLUDE_DIRS CACHE "" PATH "MaterialX include directory ")
set(MATERIALX_DEFINITIONS_DIR CACHE "" PATH "MaterialX definition directory")

# Library name custom suffix
# This helps an application that needs to ship a dynamic library MaterialX ensure
# that it has a unique name that won't conflict with one elsewhere on the system.
set (GLTFMTLX_LIBNAME_SUFFIX "" CACHE STRING "Specify a suffix to all libraries that are built")

# Do if Mac
if (APPLE)
   set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()
set(MATERIALX_EXTERNAL FALSE)
if (MATERIALX_EXTERNAL)
    set(MATERIALX_LIBRARY_NAMES "MaterialXCore;MaterialXFormat;MaterialXRender")
    find_path(MATERIALX_INCLUDE_DIRS
        NAMES 
            MaterialXCore/Document.h
        HINTS
            "${MATERIALX_ROOT}/include")

    find_path(MATERIALX_DEFINITIONS_DIR
        HINTS
            "${MATERIALX_ROOT}/libraries")
        
    foreach(libname IN ITEMS ${MATERIALX_LIBRARY_NAMES})                    
        find_library(foundLibName
            NAMES
                ${libname}
            HINTS
                "${MATERIALX_ROOT}/lib")
        LIST(APPEND MATERIALX_LIBRARIES " " ${foundLibName})
    endforeach()        
else()
    set(MATERIALX_INCLUDE_DIRS "${MATERIALX_ROOT}/include")
    set(MATERIALX_DEFINITIONS_DIR "${MATERIALX_ROOT}/libraries")
    file(GLOB MATERIALX_LIBRARIES "${MATERIALX_ROOT}/lib/*${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

message(MATERIALX_INCLUDE_DIRS: ${MATERIALX_INCLUDE_DIRS})
message(MATERIALX_LIBRARIES: ${MATERIALX_LIBRARIES})

if (NOT MATERIALX_INCLUDE_DIRS OR NOT MATERIALX_LIBRARIES)                    
    message(FATAL_ERROR "Could not find external MaterialX installation, is MATERIALX_ROOT set?")
endif()

set(GLTFMTLX_INSTALL_INCLUDE_PATH "include" CACHE STRING "Install header include path (e.g. 'inc', 'include').")
set(GLTFMTLX_INSTALL_DEFINITION_PATH "libraries" CACHE STRING "Install definition path.")
set(GLTFMTLX_INSTALL_RESOURCE_PATH "resources" CACHE STRING "Install sample resources path.")
set(GLTFMTLX_INSTALL_LIB_PATH "lib" CACHE STRING "Install lib path (e.g. 'libs', 'lib').")

add_subdirectory(source/MaterialXglTF)
add_subdirectory(source/glTF2Mtx)


    