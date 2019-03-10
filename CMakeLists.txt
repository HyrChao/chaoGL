cmake_minimum_required(VERSION 3.0)

project(chaoGL C CXX)

macro(GROUP_SRC SRCS)
    # #convert source file to absolute
    # get_filename_component(ABSOLUTE_PATH "${FILE}" ABSOLUTE)
    # # Get the directory of the absolute source file
    # get_filename_component(PARENT_DIR "${ABSOLUTE_PATH}" DIRECTORY)
    # # Remove common directory prefix to make the group
    # string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    # # Make sure we are using windows slashes
    # string(REPLACE "/" "\\" GROUP "${GROUP}")
    # # Group into "Source Files" and "Header Files"
    # if ("${FILE}" MATCHES ".*\\.cpp")
    #     set(GROUP "Source Files${GROUP}")
    # elseif("${FILE}" MATCHES ".*\\.h")
    #     set(GROUP "Header Files${GROUP}")
    # endif()
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


option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(./Includes/
                    ./3rdparty/glad/include/
                    ./3rdparty/glfw/include/
                    ./3rdparty/glm/
                    ./3rdparty/stb/
                    ./Sources/
                    )

file(GLOB_RECURSE VENDORS_SOURCES ./3rdparty/*.h)
file(GLOB_RECURSE PROJECT_HEADERS ./Includes/*.h)
file(GLOB_RECURSE PROJECT_SHADERS ./Shaders/*.fs
                                  ./Shaders/*.vs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

file(GLOB_RECURSE PROJECT_SOURCES ./Sources/*.cpp
                                  ./Sources/*.h)

set(all_src ${PROJECT_SOURCES})
GROUP_SRC(all_src)
set(all_3rdparty ${VENDORS_SOURCES})
GROUP_SRC(all_3rdparty)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                        )
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})