cmake_minimum_required(VERSION 3.23.2)

add_subdirectory("thirdParty/SFML-3.0.0")

Project(FirstGame)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(main "${MY_SOURCES}")
target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(main PRIVATE SFML::Graphics)

