# server2.0/CMakeLists.txt

# 包含子模块
add_subdirectory(dataBase)
add_subdirectory(eventHandler)
add_subdirectory(eventManager)
add_subdirectory(logSystem)
add_subdirectory(message)
add_subdirectory(msgAnalysis)
add_subdirectory(msgHandleStrategy)
add_subdirectory(MysqlPool)
add_subdirectory(networkio)
add_subdirectory(RedisPool)
add_subdirectory(serializationMethod)
add_subdirectory(socketManager)
add_subdirectory(threadPool)

# 主程序源文件
file(GLOB SOURCES "*.cpp")

# 创建可执行文件
add_executable(server2_0 ${SOURCES})

# 链接所有子模块到主程序
target_link_libraries(server2_0 
    dataBase
    eventHandler
    eventManager
    logSystem
    message
    msgAnalysis
    msgHandleStrategy
    MysqlPool
    networkio
    RedisPool
    serializationMethod
    socketManager
    threadPool
)