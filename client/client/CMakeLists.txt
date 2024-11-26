cmake_minimum_required(VERSION 3.14)
project(client)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 模块
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 包含头文件路径
include_directories(include)

# 添加可执行文件
add_executable(client 
                src/main.cpp
                src/client.cpp 
                src/user.cpp 
                include/client.h 
                include/login.hpp 
                include/mainwindow.hpp 
                include/register.hpp
                include/user.h
)

# 链接 Qt 模块
target_link_libraries(my_login_app Qt5::Core Qt5::Gui Qt5::Widgets)