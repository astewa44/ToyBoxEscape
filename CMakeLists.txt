cmake_minimum_required (VERSION 3.0)
project(ToyBoxEscape)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} cmake)
set(CMAKE_CXX_FLAGS "-O3")
include(ExternalProject)
find_package(Git REQUIRED)
find_package(SFML 2 REQUIRED system window graphics audio)
find_package(Boost 1.57.0 REQUIRED)
find_package(Box2D REQUIRED)
find_package(jsoncpp REQUIRED)
add_subdirectory(ginseng)
add_subdirectory(raspberry)
add_subdirectory(ChaiScript)

get_directory_property(CHAI_VERSION DIRECTORY ChaiScript DEFINITION CHAI_VERSION)
add_library(chaiscript INTERFACE)

# NOTE: Should probably avoid using GLOB, but it's fine for now.
file(GLOB_RECURSE toy_box_escape_SOURCES src/*)
list(REMOVE_ITEM toy_box_escape_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

add_library(lib_toy_box_escape ${toy_box_escape_SOURCES})
set_property(TARGET lib_toy_box_escape PROPERTY OUTPUT_NAME toy_box_escape)
set_property(TARGET lib_toy_box_escape PROPERTY CXX_STANDARD 14)
set_property(TARGET lib_toy_box_escape APPEND PROPERTY COMPILE_DEFINITIONS BOOST_NO_CXX11_VARIADIC_TEMPLATES)
target_include_directories(lib_toy_box_escape INTERFACE src/)
target_include_directories(lib_toy_box_escape PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(lib_toy_box_escape PUBLIC ${SFML_INCLUDE_DIR})
target_include_directories(lib_toy_box_escape PUBLIC ${BOX2D_INCLUDE_DIRS})
target_include_directories(chaiscript INTERFACE ChaiScript/include)
target_link_libraries(lib_toy_box_escape ${SFML_LIBRARIES})
target_link_libraries(lib_toy_box_escape ${BOX2D_LIBRARIES})
target_link_libraries(lib_toy_box_escape jsoncpp)
target_link_libraries(lib_toy_box_escape ginseng)
target_link_libraries(lib_toy_box_escape raspberry)
target_link_libraries(lib_toy_box_escape chaiscript)

add_executable(toy_box_escape "src/Main.cpp")
set_property(TARGET toy_box_escape PROPERTY CXX_STANDARD 14)
set_property(TARGET toy_box_escape APPEND PROPERTY COMPILE_DEFINITIONS BOOST_NO_CXX11_VARIADIC_TEMPLATES)
target_link_libraries(toy_box_escape lib_toy_box_escape)

ExternalProject_Add(
    catch_external
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 5
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
set_target_properties(catch_external PROPERTIES EXCLUDE_FROM_ALL TRUE)
ExternalProject_Get_Property(catch_external source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include)
unset(source_dir)

file(GLOB_RECURSE test_toy_box_escape_SOURCES test/*)
add_executable(test_toy_box_escape EXCLUDE_FROM_ALL ${test_toy_box_escape_SOURCES})
set_property(TARGET test_toy_box_escape PROPERTY CXX_STANDARD 14)
set_property(TARGET test_toy_box_escape APPEND PROPERTY COMPILE_DEFINITIONS BOOST_NO_CXX11_VARIADIC_TEMPLATES)
target_include_directories(test_toy_box_escape PRIVATE ${CATCH_INCLUDE_DIR})
target_link_libraries(test_toy_box_escape lib_toy_box_escape)
add_dependencies(test_toy_box_escape catch_external)
