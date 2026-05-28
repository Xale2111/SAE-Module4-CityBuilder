#CMake version
cmake_minimum_required(VERSION 3.30)

#Create solution with name
project(SAE-AlexK-CityBuilder)

#C++ Version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Creating architecture and libraries
add_subdirectory(core/)
add_subdirectory(api/)
add_subdirectory(game/)

add_custom_target(copy_resources ALL
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/_assets
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/_assets ${CMAKE_BINARY_DIR}/_assets
            COMMENT "🔄 Copying all resources to build directory"
)

#Executable target
#(exe_name file)
add_executable(CityBuilder main.cc)
add_dependencies(CityBuilder copy_resources)

#links the library 'game' to the executable
target_link_libraries(CityBuilder PRIVATE game api core)