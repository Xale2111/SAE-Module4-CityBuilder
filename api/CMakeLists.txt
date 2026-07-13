find_package(SFML COMPONENTS Graphics Window System Audio CONFIG REQUIRED)

#Creating variables for the path of our files
file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

#Create library with files (using the variables)
add_library(api ${SRC_FILES} ${HEADER_FILES})

#set the include folder as the header folder
target_include_directories(api PUBLIC include/)
target_link_libraries(api PUBLIC SFML::System SFML::Graphics SFML::Window SFML::Audio nlohmann_json)
target_link_libraries(api PUBLIC core)

target_link_libraries(api PRIVATE TracyClient)