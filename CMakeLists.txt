cmake_minimum_required(VERSION 3.10)

# set the project name
project(glTF2Mtlx VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Get MaterialX installation information
#set(MATERIALX_ROOT "<MATERIALX_ROOT>" CACHE PATH "Path to the root folder of the MaterialX installation.")
set(MATERIALX_ROOT "D:/Work/materialx/bernard_MaterialX_2/build/installed" CACHE PATH "Path to the root folder of the MaterialX installation.")
message(MATERIALX_ROOT: ${MATERIALX_ROOT})
if (NOT EXISTS ${MATERIALX_ROOT})
    message(FATAL_ERROR "Could not find external MaterialX installation: ${MATERIAL_ROOT}")
endif()

set(MATERIALX_LIBRARIES "" PATH)
set(MATERIALX_INCLUDE_DIR "" PATH)

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
    file(GLOB MATERIALX_LIBRARIES "${MATERIALX_ROOT}/lib/*${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

message(MATERIALX_INCLUDE_DIRS: ${MATERIALX_INCLUDE_DIRS})
message(MATERIALX_LIBRARIES: ${MATERIALX_LIBRARIES})

if (NOT MATERIALX_INCLUDE_DIRS OR NOT MATERIALX_LIBRARIES)                    
    message(FATAL_ERROR "Could not find external MaterialX installation, is MATERIALX_ROOT set?")
endif()

file(GLOB materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB materialx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

add_executable(glTF2Mtlx ${materialx_source} ${materialx_headers})

#set(MATERIALX_LIBRARIES
#     MaterialXFormat,
#     MaterialXRender)

target_link_libraries(
    glTF2Mtlx
    ${MATERIALX_LIBRARIES})

target_include_directories(
    glTF2Mtlx
    PRIVATE
    ${CMAKE_BINARY_DIR},
    ${MATERIALX_INCLUDE_DIRS})

install(TARGETS glTF2Mtlx
    RUNTIME DESTINATION bin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/glTF2Mtlx.pdb"
    DESTINATION "bin" OPTIONAL)

    