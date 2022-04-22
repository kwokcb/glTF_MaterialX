file(GLOB materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB materialx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

add_executable(glTF2Mtlx ${materialx_source} ${materialx_headers})

target_link_libraries(
    glTF2Mtlx
    MaterialXglTF
    ${MATERIALX_LIBRARIES})

target_include_directories(glTF2Mtlx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${GLTFMTLX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        ${CMAKE_BINARY_DIR},
        ${MATERIALX_INCLUDE_DIRS})

#install(TARGETS glTF2Mtlx
#    EXPORT MaterialX
#    ARCHIVE DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
#    LIBRARY DESTINATION ${GLTFMTLX_INSTALL_LIB_PATH}
#    RUNTIME DESTINATION bin)

 #install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
 #   DESTINATION ${GLTFMTLX_INSTALL_INCLUDE_PATH}/MaterialXglTF/ 
 #   FILES_MATCHING PATTERN "*.h*")   

install(TARGETS glTF2Mtlx
    EXPORT MaterialX
    RUNTIME DESTINATION bin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/glTF2Mtlx.pdb"
        DESTINATION "bin" OPTIONAL)
