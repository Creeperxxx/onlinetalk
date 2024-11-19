# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyServerProject)

# 添加可执行文件，指定源文件
add_executable(my_server main.cpp server.cpp onlineio.cpp)

# 包含头文件目录（如果头文件不在当前目录）
include_directories(${PROJECT_SOURCE_DIR})

# 启用调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")