find_package(SFML COMPONENTS Graphics Window System Audio CONFIG REQUIRED)

#Creating variables for the path of our files
file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

#Create library with files (using the variables)
add_library(game ${SRC_FILES} ${HEADER_FILES})

#set the include folder as the header folder
target_include_directories(game PUBLIC include/)
target_link_libraries(game PUBLIC SFML::System SFML::Graphics SFML::Window SFML::Audio)
target_link_libraries(game PUBLIC api)