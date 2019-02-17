#为项目配置CMake的总入口，设置一些全局的变量
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project (SUMMARY_DEBUG)

#SUMMARY_DEBUG_SOURCE_DIR这个变量是在指定项目名称为SUMMARY_DEBUG后自动生成的变量，
#指的是我们在CMake GUI中为项目指定的源目录

#设置GLFW库的头文件目录
set(GLFW3_INCLUDE_DIR ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/GLFW)
#设置GLFW库的库文件路径
set(GLFW3_LIBRARY ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/x64/libs/glfw3.lib)

#设置GLEW库的头文件目录
set(GLEW_INCLUDE_DIR ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/GLEW)
#设置GLEW库的库文件目录
set(GLEW_LIBRARY ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/x64/libs/glew32.lib)

#设置glm库的头文件目录，改库不需要预先进行编译，可以直接使用
set(GLM_INCLUDE_DIRS ${SUMMARY_DEBUG_SOURCE_DIR}/opengl_helper/include/glm)

#将上述的头文件目录添加到一个列表变量之中
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLM_INCLUDE_DIRS} )
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLFW3_INCLUDE_DIR} )
list( APPEND SUMMARY_DEBUG_INCLUDE ${GLEW_INCLUDE_DIR} )

#将上述的库文件目录添加到一个列表变量之中
list( APPEND SUMMARY_DEBUG_LIBS ${GLFW3_LIBRARY} )
list( APPEND SUMMARY_DEBUG_LIBS ${GLEW_LIBRARY} )

#利用find_package命令找到OpenGL
find_package( OpenGL REQUIRED )
list( APPEND SUMMARY_DEBUG_LIBS ${OPENGL_gl_LIBRARY} )

#设置项目的头文件包含目录
include_directories( ${SUMMARY_DEBUG_INCLUDE} )

#设置子目录
add_subdirectory(main_project)
