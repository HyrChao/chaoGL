cmake_minimum_required(VERSION 3.0)


project(chaoGL C CXX)

#set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

macro(GROUP_SRC SRCS)
    # source_group("${GROUP}" FILES "${FILE}")
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(sgbd_file ${${SRCS}})
    string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
    string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
    string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
    string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
    string(REPLACE "." "" sgbd_group_name ${sgbd_group_name})
    # if ("${FILE}" MATCHES ".*\\.cpp")
    #     set(GROUP "Source Files${GROUP}")
    # elseif("${FILE}" MATCHES ".*\\.h")
    #     set(GROUP "Header Files${GROUP}")
    # endif()
    if(sgbd_nogroup)
    set(sgbd_group_name "\\")
    endif(sgbd_nogroup)
    source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endmacro(GROUP_SRC)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)

# assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(3rdparty/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(./Includes/
./3rdparty/assimp/include/
./3rdparty/glad/include/
./3rdparty/glfw/include/
./3rdparty/glm/
./3rdparty/stb/
./Sources/
)

file(GLOB VENDORS_SOURCES ./3rdparty/glad/src/glad.c)
file(GLOB_RECURSE PROJECT_HEADERS ./Includes/*.h)
file(GLOB_RECURSE PROJECT_SHADERS ./Shaders/*.fs
./Shaders/*.vs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
Readme.md
.gitattributes
.gitignore
.gitmodules)
file(GLOB_RECURSE VENDORS_HEADERS ./3rdparty/*.h
./3rdparty/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ./Sources/*.cpp
./Sources/*.h)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-DGLFW_INCLUDE_NONE
-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
${PROJECT_SHADERS} ${PROJECT_CONFIGS}
${VENDORS_SOURCES} ${VENDORS_HEADERS})
target_link_libraries(${PROJECT_NAME} assimp glfw
${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})


source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("3rdpartySource" FILES ${VENDORS_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})
set(all_src ${PROJECT_SOURCES})
GROUP_SRC(all_src)
set(all_3rdparty ${VENDORS_HEADERS})
GROUP_SRC(all_3rdparty)


#This executes every time you build ${PROJECT_NAME} 
#and copies the contents of "/config" into the directory where the target exe/lib will end up.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders)

# No need to copy assets after defined root by cmake
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                        ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)
# Include config files from cmake
configure_file(Config/CMake/root_directory.h.in Config/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/Config)     
