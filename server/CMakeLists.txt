# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyServerProject)

# 添加可执行文件，指定源文件
add_executable(my_server main.cpp server.cpp onlineio.cpp msganalyse.cpp mysqlpool.cpp taskpackage.cpp threadpool.cpp user.cpp)


# 包含头文件目录（如果头文件不在当前目录）
include_directories(${PROJECT_SOURCE_DIR})

# 启用调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 指定模块路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 查找 MySQL Connector/C++ 库
find_package(MySQLConnectorCpp REQUIRED)

# 链接 MySQL Connector/C++ 库
target_link_libraries(my_server ${MYSQLCONNECTORCPP_LIBRARIES})

# 添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()