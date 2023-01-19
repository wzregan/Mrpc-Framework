cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE "Debug") # 设置debug版本，可以进行调试了
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #设置可执行文件的输出目录 
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) # 设置动态库的输出木兰

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

link_directories(${PROJECT_SOURCE_DIR}/lib) # 库的搜索路径


include_directories(${PROJECT_SOURCE_DIR}/src/include) # 包含目录


# 添加子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/test) 
add_subdirectory(${PROJECT_SOURCE_DIR}/example/calle)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/example/caller)