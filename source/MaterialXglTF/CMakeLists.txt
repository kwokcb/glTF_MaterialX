file(GLOB materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB materialx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

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
    ${MATERIALX_LIBRARIES})

target_include_directories(MaterialXglTF
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${GLTFMTLX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        ${CMAKE_BINARY_DIR},
        ${MATERIALX_INCLUDE_DIRS})

install(TARGETS MaterialXglTF
    EXPORT MaterialX
    ARCHIVE DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
    LIBRARY DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
    RUNTIME DESTINATION bin)

 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION ${GLTFMTLX_INSTALL_INCLUDE_PATH}/MaterialXglTF/ 
    FILES_MATCHING PATTERN "*.h*")   

add_custom_command(TARGET MaterialXglTF
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/../../resources ${CMAKE_BINARY_DIR}/bin/resources)
