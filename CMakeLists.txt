##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
list                  (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/CMakePCHCompiler ${CMAKE_CURRENT_LIST_DIR}/cmake)
project               (type_ext VERSION 0.1 LANGUAGES CXX CXXPCH)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 17)
include               (set_max_warning_level)
set_max_warning_level ()

##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_FILES
        ${PROJECT_HEADERS}
        ${PROJECT_CMAKE_UTILS}
        ${PROJECT_MISC})

include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(import_library)
# ADD LIBRARIES HERE.
# Examples:
# - Header Only:
#   find_package  (GLM REQUIRED)
#   import_library(GLM_INCLUDE_DIRS)
# - Identical Debug and Release:
#   find_package  (OpenGL REQUIRED)
#   import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
# - Separate Debug and Release:
#   find_package  (Boost REQUIRED iostreams)
#   import_library(Boost_INCLUDE_DIRS Boost_IOSTREAMS_LIBRARY_DEBUG Boost_IOSTREAMS_LIBRARY_RELEASE)
# - You may also set the PROJECT_INCLUDE_DIRS and PROJECT_LIBRARIES instead of using import_library.

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} INTERFACE ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} INTERFACE ${PROJECT_COMPILE_DEFINITIONS})

# Hack for header-only project to appear in the IDEs.
add_library(${PROJECT_NAME}_ STATIC ${PROJECT_FILES})
target_include_directories(${PROJECT_NAME}_ PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME}_ PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME}_ PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME}_ PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME}_ PROPERTIES LINKER_LANGUAGE CXX)
#target_precompiled_header(${PROJECT_NAME}_  ${CMAKE_CURRENT_SOURCE_DIR}/include/type_ext/type_ext.h)


##################################################    Testing     ##################################################
#if(BUILD_TESTS)
enable_testing     ()
add_executable(tuple_slice_test tests/type_ext_test.cpp) # tests/doctest/main.cpp)
add_test(tuple_slice_test tuple_slice_test)
target_link_libraries(tuple_slice_test type_ext )
target_compile_options(tuple_slice_test PRIVATE $<$<CXX_COMPILER_ID:Clang>:-std=gnu++17 -Wall>)
#target_compile_options(statemachine_test PRIVATE -ftime-report)

#    set                (TEST_MAIN_NAME doctest_main)
#    set                (TEST_MAIN_SOURCES tests/doctest/main.cpp)
#    add_library        (${TEST_MAIN_NAME} OBJECT ${TEST_MAIN_SOURCES})
#    set_property       (TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests/doctest)
#    assign_source_group(${TEST_MAIN_SOURCES})
#
#    file(GLOB PROJECT_TEST_CPPS tests/*.cpp)
#    foreach(_SOURCE ${PROJECT_TEST_CPPS})
#        get_filename_component(_NAME ${_SOURCE} NAME_WE)
#        add_executable        (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
#        target_link_libraries (${_NAME} ${PROJECT_NAME})
#        add_test              (${_NAME} ${_NAME})
#        set_property          (TARGET ${_NAME} PROPERTY FOLDER tests)
#        assign_source_group   (${_SOURCE})
#    endforeach()
#endif()

##################################################  Installation  ##################################################
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME ${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE
        ${PROJECT_NAME}Targets.cmake
        NAMESPACE
        ${PROJECT_NAME}::
        DESTINATION
        ${INSTALL_CONFIGDIR}
        )
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
        )
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR}
        )

## Exporting from the build tree

export(EXPORT ${PROJECT_NAME}-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake NAMESPACE ${PROJECT_NAME}::)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}-exports.cmake)

#Register package in user's package registry
export(PACKAGE ${PROJECT_NAME})