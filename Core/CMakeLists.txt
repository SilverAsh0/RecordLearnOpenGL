file(GLOB CORE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/Class/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Math/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Data/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Light/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AssetsLoad/*.cpp")
file(GLOB CORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(Core ${CORE_CPP} LOG.h)
#将文件夹公开链接库
target_include_directories(Core PUBLIC ${CORE_INCLUDE})

#将第三方库链接进该项目，让该子目录可以使用
target_link_libraries(Core PRIVATE glad::glad)
target_link_libraries(Core PRIVATE assimp::assimp)