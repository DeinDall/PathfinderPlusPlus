project(PathfinderPlusPlus)
cmake_minimum_required(VERSION 3.1)

# Notifying CMake where to search for additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# Requesting C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

# Defining source files
aux_source_directory(./src/ SRC_LIST)
aux_source_directory(./src/path SRC_LIST)
aux_source_directory(./src/graphics SRC_LIST)
aux_source_directory(./src/graphics/components SRC_LIST)
aux_source_directory(./src/util SRC_LIST)
aux_source_directory(./src/tinyfiledialogs SRC_LIST)

# Defining executable
add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})

# Finding SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.3 COMPONENTS graphics window system main)

# Finding SFGUI
set(SFGUI_STATIC_LIBRARIES TRUE)
find_package(SFGUI 0.3 REQUIRED)

# Including & Linking SFML & SFGUI
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARY} ${SFGUI_DEPENDENCIES} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

# Link dialog lib
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} "comdlg32")
endif()

# Specify we want to link the standard library STATICALLY
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Move the resource files
file(COPY ./resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
