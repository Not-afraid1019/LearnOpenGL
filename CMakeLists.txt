cmake_minimum_required(VERSION 3.28)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DDEBUG)

# 把需要拷贝的资源路径都放到ASSETS里
file(GLOB ASSETS "./assets")

# 把ASSETS指代的目录集合的内容，都拷贝到可执行文件目录下
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdPart/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdPart/libs)

add_subdirectory(wrapper)

add_subdirectory(application)

add_subdirectory(glframework)

add_executable(LearnOpenGL main.cpp glad.c)

target_link_libraries(LearnOpenGL glfw3.lib wrapper app framework)
