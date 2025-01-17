cmake_minimum_required(VERSION 3.29)

project(map)

add_library(${PROJECT_NAME}_obj OBJECT "src/hash.c" "src/utils.c" "src/hashmap.c")
target_include_directories(${PROJECT_NAME}_obj PUBLIC "include")
target_include_directories(${PROJECT_NAME}_obj PRIVATE "src/include")
target_compile_options(${PROJECT_NAME}_obj PUBLIC "-g" "-fPIC" "-O2")
target_link_libraries(${PROJECT_NAME}_obj PUBLIC "m")

add_library(${PROJECT_NAME}_shared SHARED)
target_include_directories(${PROJECT_NAME}_shared PUBLIC "include")
target_link_libraries(${PROJECT_NAME}_shared ${PROJECT_NAME}_obj)
target_compile_options(${PROJECT_NAME}_shared PUBLIC "-fPIC")

add_library(${PROJECT_NAME}_static STATIC)
target_include_directories(${PROJECT_NAME}_static PUBLIC "include")
target_link_libraries(${PROJECT_NAME}_static  ${PROJECT_NAME}_obj)

add_executable("test_map" "test/test.c" "test/timing.c")
target_include_directories("test_map" PUBLIC "include")
target_link_libraries("test_map" ${PROJECT_NAME}_obj)
target_compile_options("test_map" PUBLIC "-g")

add_executable("test_map2" "test/test2.c" "test/timing.c")
target_include_directories("test_map2" PUBLIC "include")
target_link_libraries("test_map2" ${PROJECT_NAME}_obj)
target_compile_options("test_map2" PUBLIC "-g")