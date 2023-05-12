# CMakeLists.txt
cmake_minimum_required(VERSION 3.25)
project(OpenGL)
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#add_compile_options(/W4 /WX /w44263)

#add_subdirectory(Function)

#find package
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")
find_package(assimp CONFIG REQUIRED)

#添加子目录的cmakelists文件，注意在find之后使用，因为子目录依赖了find
add_subdirectory(Core)
#Editor有依赖Core
#add_subdirectory(Editor)
#generate executable
add_executable(OpenGL main.cpp)

#link libraries
#public 还是private 决定了子目录的cmakelists项目是否可以访问该库文件或者头文件
target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC Core)
#target_link_libraries(${PROJECT_NAME} PUBLIC Editor)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp::assimp)
#添加头文件，将头文件包含在库或者可执行文件中，（ep，某些库只有头文件）
target_include_directories(${PROJECT_NAME} PUBLIC ${RAPIDXML_INCLUDE_DIRS})

#告诉cmake在这个目录寻找需要的libs
#target_link_directories(${PROJECT_NAME}  Core)
#将指定lib链接进项目,对其他链接进该项目的lib是否公开
#target_link_libraries(${PROJECT_NAME} PUBLIC Core)