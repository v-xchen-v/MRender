cmake_minimum_required ( VERSION 3.0)       # 支持最低的CMake版本
project                ( MRender )              # 我的工程名叫MRender

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )      # 生成 compile_commands.json供vim c++插件语义补全使用

# 下面几行设置c++编译选项
set(CXX_FLAGS -std=c++11 -g -Wall)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG " )

# 可选，设置可执行程序的输出目录，我习惯于放在build/bin. 库放在build/lib下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/build/lib)

# 下面三个开关用来控制glfw的构建，关闭 帮助文档、测试项目、示例项目的构建。因为我只是把glfw作为一个库来使用。
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)  
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# 添加glfw的CMake模块。 相当于 include(./glfw/CMakeLists.txt), glfw使用自己的构建脚本。
# 同时会把glfw的头文件包含目录添加到项目中。
add_subdirectory(${PROJECT_SOURCE_DIR}/glfw)

# 添加 ./src 到头文件搜索路径，以后自己编写的头文件如果放在src下的话，就能被编译器搜索到。
include_directories ( ${PROJECT_SOURCE_DIR}/src)

# 添加要编译的cpp源代码，先只要一个main.cpp
set(SRC_LIST ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 添加项目的构建目标，一个可执行程序，叫：${PROJECT_NAME}, 也就是文件开头定义的 “MRender”
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 让项目的可执行程序red链接glfw库
target_link_libraries(${PROJECT_NAME} glfw)

# 让CMake去系统搜索OpenGL库，找不到就报错(REQUIRED).  
# 找到了之后就会有两个变量可用：OPENGL_INCLUDE_DIR 和 OPENGL_gl_LIBRARY
find_package(OpenGL REQUIRED)

# 添加OpenGL头文件包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

# 让项目的可执行程序red链接系统OpenGL库
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})