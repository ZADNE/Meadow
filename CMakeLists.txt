cmake_minimum_required(VERSION 3.24)
project(Meadow)

#RealWorld Game
add_executable_with_shaders(Meadow)
target_include_directories(Meadow PRIVATE src)
set_target_properties(Meadow PROPERTIES
    CXX_STANDARD 20
    REALSHADERS_CXX_NAMESPACE "md"
)
target_link_libraries(Meadow PUBLIC RealEngine)
add_custom_command(TARGET Meadow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data/ .
)

#Doxygen documentation
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(Meadow_Documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
    )
endif (DOXYGEN_FOUND)

#Add subdirectories
add_subdirectory(src)
