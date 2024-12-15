# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyServerProject VERSION 1.0 LANGUAGES CXX)
# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # 查找 hiredis 使用 pkg-config
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(HIREDIS REQUIRED IMPORTED_TARGET hiredis)

# # 查找 MySQL Connector/C++ 使用 pkg-config（假设名为 mysqlcppconn）
# pkg_check_modules(MYSQLCPPCONN REQUIRED IMPORTED_TARGET mysqlcppconn)

# # 设置 MySQL Connector/C++ 的安装路径
# set(CMAKE_PREFIX_PATH "/path/to/mysql-connector-cpp")

# # 查找 MySQL Connector/C++
# find_package(mysqlcppconn REQUIRED)
# if (mysqlcppconn_FOUND)
#     message(STATUS "Found MySQL Connector/C++: ${MYSQLCPPCONN_INCLUDE_DIRS}")
# else()
#     message(FATAL_ERROR "MySQL Connector/C++ not found")
# endif()

# # 查找 hiredis
# find_package(hiredis REQUIRED)
# if (HIREDIS_FOUND)
#     message(STATUS "Found hiredis: ${HIREDIS_INCLUDE_DIRS}")
# else()
#     message(FATAL_ERROR "hiredis not found")
# endif()

# 查找 MySQL SDK
#set(MYSQLCLIENT_STATIC_LINKING true)
#set(FINDMYSQL_DEBUG true)
# find_package(MySQL MODULE REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 现在你可以使用 find_package 来查找 MySQL Connector/C++
find_package(MySQLConnectorCpp REQUIRED)

# 查找MySQL Connector/C++
if(MYSQLCONNECTORCPP_FOUND)
    message(STATUS "Found MySQL Connector/C++: ${MYSQLCONNECTORCPP_INCLUDE_DIRS}, ${MYSQLCONNECTORCPP_LIBRARIES}")
endif()

# 现在你可以使用 find_package 来查找 hiredis
find_package(Hiredis REQUIRED)

if(HIREDIS_FOUND)
    message(STATUS "Found hiredis: ${HIREDIS_INCLUDE_DIRS}, ${HIREDIS_LIBRARIES}")
endif()

# 查找 zlib 库
find_package(ZLIB REQUIRED)

# # 设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 收集所有源文件
file(GLOB_RECURSE SOURCES
    server2.0/dataBase/*.cpp
    server2.0/eventHandler/*.cpp
    server2.0/eventManager/*.cpp
    server2.0/logSystem/*.cpp
    server2.0/msgAnalysis/*.cpp
    server2.0/msgHandleStrategy/*.cpp
    server2.0/MysqlPool/*.cpp
    server2.0/networkio/*.cpp
    server2.0/RedisPool/*.cpp
    server2.0/serializationMethod/*.cpp
    server2.0/socketManager/*.cpp
    server2.0/threadPool/*.cpp
    server2.0/main.cpp
    # server2.0/requirement/redisconnect/*.cpp
)
# set(SOURCES 
#     server2.0/dataBase/dataBase.cpp
#     server2.0/eventHandler/eventHandler.cpp 
#     server2.0/eventManager/eventManager.cpp 
#     server2.0/logSystem/log.cpp 
#     server2.0/msgAnalysis/msgAnalysisFSM.cpp 
#     server2.0/msgAnalysis/msgAnalysis.cpp 
#     server2.0/msgHandleStrategy/msgHandleStrategy.cpp 
#     server2.0/MysqlPool/mysqlMethods.cpp 
#     server2.0/MysqlPool/mysqlpool.cpp 
#     server2.0/networkio/networkio.cpp
#     server2.0/RedisPool/RedisMethods.cpp 
#     server2.0/RedisPool/RedisPool.cpp
#     server2.0/serializationMethod/serializationMethod.cpp
#     server2.0/socketManager/socketManager.cpp 
#     server2.0/socketManager/socketVec.cpp 
#     server2.0/threadPool/threadPool.cpp 
#     server2.0/threadPool/task.cpp 
#     server2.0/main.cpp
# )

set(HEADERS 

    ${CMAKE_SOURCE_DIR}/server2.0/dataBase
    ${CMAKE_SOURCE_DIR}/server2.0/eventHandler
    ${CMAKE_SOURCE_DIR}/server2.0/eventManager
    ${CMAKE_SOURCE_DIR}/server2.0/logSystem
    ${CMAKE_SOURCE_DIR}/server2.0/msgAnalysis
    ${CMAKE_SOURCE_DIR}/server2.0/msgHandleStrategy
    ${CMAKE_SOURCE_DIR}/server2.0/MysqlPool
    ${CMAKE_SOURCE_DIR}/server2.0/networkio
    ${CMAKE_SOURCE_DIR}/server2.0/RedisPool
    ${CMAKE_SOURCE_DIR}/server2.0/serializationMethod
    ${CMAKE_SOURCE_DIR}/server2.0/socketManager
    ${CMAKE_SOURCE_DIR}/server2.0/threadPool
    ${CMAKE_SOURCE_DIR}/server2.0/message
    ${CMAKE_SOURCE_DIR}/server2.0/requirement/moodycamel
    # ${CMAKE_SOURCE_DIR}/server2.0/requirement/redisconnect
    # ${MYSQLCPPCONN_INCLUDE_DIRS}
    # ${HIREDIS_INCLUDE_DIRS}
    ${MYSQLCONNECTORCPP_INCLUDE_DIRS}
    ${HIREDIS_INCLUDE_DIRS}
    # 包含 zlib 的头文件目录
    ${ZLIB_INCLUDE_DIRS}
)

add_executable(server2.0 ${SOURCES})

# 链接MySQL Connector/C++ 和 Hiredis库
target_link_libraries(server2.0 PRIVATE ${MYSQLCONNECTORCPP_LIBRARIES} ${HIREDIS_LIBRARIES} ${ZLIB_LIBRARIES})

# # 链接 MySQL Connector/C++ 和 hiredis 库
# target_link_libraries(my_project PRIVATE
#     ${MYSQLCPPCONN_LIBRARIES}
#     ${HIREDIS_LIBRARIES}
# )


# 链接 hiredis 库
# target_link_libraries(my_project PRIVATE PkgConfig::HIREDIS)

# 链接 MySQL Connector/C++ 库
# target_link_libraries(my_project PRIVATE PkgConfig::MYSQLCPPCONN)

target_include_directories(server2.0 PRIVATE ${HEADERS})

# 添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()




# # # 查找 src 目录下的所有 .cpp 文件
# # file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/server2.0/**/*.cpp")
# # # 添加可执行文件，指定源文件
# # add_executable(my_server ${SOURCES})

# # # 包含头文件目录（如果头文件不在当前目录）
# # # include_directories(${PROJECT_SOURCE_DIR}/include)
# # # file(GLOB_RECURSE INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/server2.0/*")
# # # list(REMOVE_DUPLICATES INCLUDE_DIRS)
# # # target_include_directories(my_server PRIVATE ${INCLUDE_DIRS})

# # # 递归获取 server 下的所有子目录
# # file(GLOB_RECURSE SERVER_INCLUDE_DIRS LIST_DIRECTORIES true "${PROJECT_SOURCE_DIR}/server/server2.0/*" "${PROJECT_SOURCE_DIR}/server/server2.0/requirements/*")


# # # 过滤出仅包含目录的路径
# # set(SERVER_INCLUDE_DIRS_FILTERED "")
# # foreach(dir IN LISTS SERVER_INCLUDE_DIRS)
# #     if(IS_DIRECTORY ${dir})
# #         list(APPEND SERVER_INCLUDE_DIRS_FILTERED ${dir})
# #     endif()
# # endforeach()

# # # 将过滤后的子目录添加为包含路径
# # target_include_directories(my_server PRIVATE ${SERVER_INCLUDE_DIRS_FILTERED})


# # # 启用调试信息
# # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# # # 指定模块路径
# # list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# # # 查找 MySQL Connector/C++ 库
# # find_package(MySQLConnectorCpp REQUIRED)

# # # 链接 MySQL Connector/C++ 库
# # target_link_libraries(my_server ${MYSQLCONNECTORCPP_LIBRARIES})




# # cmake_minimum_required(VERSION 3.10)

# # # 设置项目名称和版本
# # project(MyServerProject VERSION 1.0)

# # # 指定使用的 C++ 标准
# # set(CMAKE_CXX_STANDARD 17)
# # set(CMAKE_CXX_STANDARD_REQUIRED True)

# # # 设置输出路径
# # set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# # # 包含当前目录（为了找到顶层头文件）
# # include_directories(${PROJECT_SOURCE_DIR})

# # # 如果需要外部库或依赖，可以在这里添加 find_package 命令
# # # find_package(MySQLConnectorCpp REQUIRED)
# # list(APPEND CMAKE_PREFIX_PATH /etc/redis)  
# # # find_package(Redis REQUIRED)

# # # 包含 cmake 模块
# # include(cmake/FindMySQLConnectorCpp.cmake)

# # # 函数用于递归添加子目录中的源文件
# # function(add_sources_to_target TARGET_NAME BASE_DIR)
# #     file(GLOB_RECURSE SOURCES RELATIVE ${BASE_DIR} "${BASE_DIR}/*.cpp")
# #     target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
# # endfunction()

# # # 创建一个静态库目标，用于收集所有模块
# # add_library(server2_0 STATIC)

# # # 添加 server2.0 下的所有 .cpp 文件到静态库中
# # add_sources_to_target(server2_0 ${CMAKE_CURRENT_SOURCE_DIR}/server2.0)

# # # 包含 server2.0 下的所有头文件目录
# # target_include_directories(server2_0 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/server2.0)

# # # 构建最终的可执行文件 (假设有一个 main.cpp 或类似的入口点)
# # add_executable(MyServerExecutable main.cpp)

# # # 链接静态库和其他所需的库
# # target_link_libraries(MyServerExecutable PRIVATE server2_0 MySQL::MySQLConnectorCpp Redis::Redis)

# # # 添加 build 和其他非源码目录到清理列表中
# # set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "build;${EXECUTABLE_OUTPUT_PATH}")

# # # 如果有额外的资源文件或其他需要复制到构建目录的文件，可以在这里处理
# # file(COPY readme DESTINATION ${PROJECT_BINARY_DIR})
# # file(COPY server.code-workspace DESTINATION ${PROJECT_BINARY_DIR})


# # # CMake最低版本要求
# # cmake_minimum_required(VERSION 3.10)

# # # 项目名称
# # project(Server2_0)

# # # 设置C++标准，这里假设使用C++11，你可按需修改
# # set(CMAKE_CXX_STANDARD 20)
# # set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # # 配置头文件搜索路径，添加项目根目录下的各个子目录
# # include_directories(
# #     ${PROJECT_SOURCE_DIR}/server2.0/dataBase
# #     ${PROJECT_SOURCE_DIR}/server2.0/eventHandler
# #     ${PROJECT_SOURCE_DIR}/server2.0/eventManager
# #     ${PROJECT_SOURCE_DIR}/server2.0/logSystem
# #     ${PROJECT_SOURCE_DIR}/server2.0/message
# #     ${PROJECT_SOURCE_DIR}/server2.0/msgAnalysis
# #     ${PROJECT_SOURCE_DIR}/server2.0/msgHandleStrategy
# #     ${PROJECT_SOURCE_DIR}/server2.0/MysqlPool
# #     ${PROJECT_SOURCE_DIR}/server2.0/networkio
# #     ${PROJECT_SOURCE_DIR}/server2.0/RedisPool
# #     ${PROJECT_SOURCE_DIR}/server2.0/serializationMethod
# #     ${PROJECT_SOURCE_DIR}/server2.0/socketManager
# #     ${PROJECT_SOURCE_DIR}/server2.0/threadPool
# # )



# # # 添加可执行文件目标
# # add_executable(Server2_0 ${SOURCE_FILES})

# # # 查找MySQL Connector C++库，如果找到则链接
# # find_package(MySQLConnectorCpp REQUIRED)
# # if (MySQLConnectorCpp_FOUND)
# #     target_link_libraries(Server2_0 MySQLConnectorCpp::MySQLConnectorCpp)
# # endif()

# # # 如果需要链接其他第三方库，可以在这里按照类似方式查找并链接
# # # 例如，假设你还需要链接Redis库（这里只是示例，具体名称等按实际情况修改）
# # # find_package(Redis REQUIRED)
# # # if (Redis_FOUND)
# # #    target_link_libraries(Server2_0 Redis::Redis)
# # # endif()

# # # 构建之后将可执行文件安装到指定目录（可选步骤，可按需决定是否添加）
# # install(TARGETS Server2_0 DESTINATION bin)


# cmake_minimum_required(VERSION 3.10)

# # 项目名称和版本号
# project(ServerProject VERSION 1.0)

# # 设置C++标准
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # 包含外部库的路径
# include_directories(
#     ${PROJECT_SOURCE_DIR}/server2.0/requirement/cereal
#     ${PROJECT_SOURCE_DIR}/server2.0/moodycamel
# )

# # 添加子目录
# add_subdirectory(server2.0)

# # 安装指令（如果需要）
# # install(TARGETS ${PROJECT_NAME} DESTINATION bin)