file(GLOB gltfMtlx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB gltfMtlx_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
list(APPEND gltfMtlx_headers "${CMAKE_CURRENT_SOURCE_DIR}/Catch/catch.hpp")

assign_source_group("Source Files" ${gltfMtlx_source})
assign_source_group("Header Files" ${gltfMtlx_headers})

# Discover all tests and allow them to be run in parallel (ctest -j20):
function(add_tests _sources)
  foreach(src_file ${_sources})
    file(STRINGS ${src_file} matched_lines REGEX "TEST_CASE")
    foreach(matched_line ${matched_lines})
      string(REGEX REPLACE "(TEST_CASE[( \"]+)" "" test_name ${matched_line})
      string(REGEX REPLACE "(\".*)" "" test_name ${test_name})
      string(REGEX REPLACE "[^A-Za-z0-9_]+" "_" test_safe_name ${test_name})
      add_test(NAME "glTFTest_${test_safe_name}"
          COMMAND glTFTest ${test_name}
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endforeach()
  endforeach()
endfunction()

add_executable(glTFTest ${gltfMtlx_source} ${gltfMtlx_headers})

target_include_directories( glTFTest PUBLIC
    ${EXTERNAL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${MATERIALX_INCLUDE_DIRS})

message(STATUS "************ current dir:" ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(MaterialXglTF)
target_link_libraries(glTFTest MaterialXglTF)

#add_custom_command(TARGET glTFTest POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	${CMAKE_CURRENT_SOURCE_DIR}/../../libraries ${CMAKE_BINARY_DIR}/bin/libraries)

#add_custom_command(TARGET glTFTest POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	${CMAKE_CURRENT_SOURCE_DIR}/../../resources ${CMAKE_BINARY_DIR}/bin/resources)

set_target_properties(
    glTFTest PROPERTIES
    OUTPUT_NAME glTFTest
    COMPILE_FLAGS "${EXTERNAL_COMPILE_FLAGS}"
    LINK_FLAGS "${EXTERNAL_LINK_FLAGS}"
    VERSION "${gltfMtlx_LIBRARY_VERSION}"
    SOVERSION "${gltfMtlx_MAJOR_VERSION}")

target_link_libraries(
    glTFTest
    ${CMAKE_DL_LIBS})
