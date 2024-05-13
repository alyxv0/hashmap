cmake_minimum_required(VERSION 3.29)

project(map)

add_library(${PROJECT_NAME}_obj OBJECT "src/hash.c" "src/utils.c" "src/hashmap.c")
target_include_directories(${PROJECT_NAME}_obj PUBLIC "include")
target_include_directories(${PROJECT_NAME}_obj PRIVATE "src/include")
target_compile_options(${PROJECT_NAME}_obj PUBLIC "-g")

add_library(${PROJECT_NAME}_shared SHARED)
target_include_directories(${PROJECT_NAME}_shared PUBLIC "include")
target_link_libraries(${PROJECT_NAME}_shared ${PROJECT_NAME}_obj)

add_library(${PROJECT_NAME}_static STATIC)
target_include_directories(${PROJECT_NAME}_static PUBLIC "include")
target_link_libraries(${PROJECT_NAME}_static  ${PROJECT_NAME}_obj)

add_executable("test_map" "test/test.c")
target_include_directories("test_map" PUBLIC "include")
target_link_libraries("test_map" ${PROJECT_NAME}_obj)
target_compile_options("test_map" PUBLIC "-g")