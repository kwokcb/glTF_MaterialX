file(GLOB materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB materialx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

add_executable(glTF2Mtlx ${materialx_source} ${materialx_headers})

target_link_libraries(
    glTF2Mtlx
    MaterialXglTF)

target_include_directories(glTF2Mtlx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${GLTFMTLX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        ${CMAKE_BINARY_DIR},
        ${MATERIALX_INCLUDE_DIRS})

install(TARGETS glTF2Mtlx
    EXPORT MaterialX
    RUNTIME DESTINATION bin)

install(DIRECTORY "${MATERIALX_DEFINITIONS_DIR}"
    DESTINATION ${GLTFMTLX_INSTALL_DEFINITION_PATH}/.. 
    FILES_MATCHING PATTERN "*.mtlx*")   

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../resources"
    DESTINATION ${GLTFMTLX_INSTALL_RESOURCE_PATH}/.. )   

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/glTF2Mtlx.pdb"
        DESTINATION "bin" OPTIONAL)

#add_custom_command(TARGET glTF2Mtlx
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	${CMAKE_CURRENT_SOURCE_DIR}/../../resources ${CMAKE_BINARY_DIR}/bin/resources)
#add_custom_command(TARGET glTF2Mtlx
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	${MATERIALX_DEFINITIONS_DIR} ${CMAKE_BINARY_DIR}/bin/libraries)
