file(GLOB materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB materialx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

#if(APPLE)
#    find_library(COCOA_FRAMEWORK Cocoa)
#    find_package(OpenGL REQUIRED)
#    file(GLOB_RECURSE materialx_source_oc "${CMAKE_CURRENT_SOURCE_DIR}/*.m")
#    message("Objective C files: " ${materialx_source_oc})
#    set_source_files_properties(${materialx_source_oc} PROPERTIES
#      COMPILE_FLAGS "-x objective-c++")
#   set(materialx_source ${materialx_source} ${materialx_source_oc})
#    add_compile_options(-DGL_SILENCE_DEPRECATION)
#elseif(UNIX)
#    find_package(X11 REQUIRED)
#    # Note - can't just require the Xt component because FindX11 in cmake 3.1
#    # doesn't support it
#    if(NOT X11_Xt_FOUND)
#        message(FATAL_ERROR "Error in building MaterialXRenderGlsl: Xt was not found")
#    endif()
#
#    find_package(OpenGL REQUIRED)
#    include_directories(${X11_INCLUDE_DIR})
#endif()

# Disable OpenGL deprecation warnings on Clang.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-deprecated-declarations)
endif()

add_library(MaterialXglTF ${materialx_source} ${materialx_headers})

set_target_properties(
    MaterialXglTF PROPERTIES
    OUTPUT_NAME MaterialXglTF${GLTFMTLX_LIBNAME_SUFFIX}
    COMPILE_FLAGS "${EXTERNAL_COMPILE_FLAGS}"
    LINK_FLAGS "${EXTERNAL_LINK_FLAGS}"
    VERSION "${GLTFMTLX_LIBRARY_VERSION}"
    SOVERSION "${GLTFMTLX_MAJOR_VERSION}")

target_link_libraries(
    MaterialXglTF
    MaterialXCore
    MaterialXGenShader
    MaterialXRender
    ${CMAKE_DL_LIBS})

#set(COMMON_LIBRARIES
#    ${MATERIALX_LIBRARIES}
#    ${CMAKE_DL_LIBS})

#if(MSVC)
#    target_link_libraries(
#        MaterialXglTF
#        ${COMMON_LIBRARIES}
#        Opengl32)
#elseif(APPLE)
#    target_link_libraries(
#        MaterialXglTF
#        ${COMMON_LIBRARIES}
#        ${OPENGL_LIBRARIES}
#        "-framework Foundation"
#        "-framework Cocoa")
#elseif(UNIX)
#    target_link_libraries(
#        MaterialXglTF
#        ${COMMON_LIBRARIES}
#        ${OPENGL_LIBRARIES}
#        ${X11_LIBRARIES}
#        ${X11_Xt_LIB})
#endif()

message(STATUS "------BIN: " ${CMAKE_CURRENT_BINARY_DIR}/../)

target_include_directories(MaterialXglTF
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../external/MaterialX/source/>
        $<INSTALL_INTERFACE:${MATERIALX_INSTALL_INCLUDE_PATH}>
        $<INSTALL_INTERFACE:${GLTFMTLX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        ${CMAKE_BINARY_DIR},
        ${MATERIALX_INCLUDE_DIRS}
        ${EXTERNAL_INCLUDE_DIRS})

install(TARGETS MaterialXglTF
    EXPORT MaterialX
    ARCHIVE DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
    LIBRARY DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
    RUNTIME DESTINATION bin)

 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION ${GLTFMTLX_INSTALL_INCLUDE_PATH}/MaterialXglTF/ 
    FILES_MATCHING PATTERN "*.h*")   
