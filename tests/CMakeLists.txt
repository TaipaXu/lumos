find_package(Boost REQUIRED COMPONENTS unit_test_framework)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found for tests")
endif()

set(TEST_SOURCES
    test.cpp
)

add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests PRIVATE
    Boost::unit_test_framework
    lumosCore
)

target_include_directories(runTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
    ${LIBMAGIC_INCLUDE_DIRS}
)

add_custom_target(copy_test_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/files
        ${CMAKE_CURRENT_BINARY_DIR}/files
    COMMENT "Copying test files..."
)
add_dependencies(runTests copy_test_files)

add_test(NAME LumosTests COMMAND runTests)
