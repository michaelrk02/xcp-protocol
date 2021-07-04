cmake_minimum_required(VERSION 3.0.2)

project("xcp")
add_library(${PROJECT_NAME} SHARED
    "src/call.c"
    "src/client.c"
    "src/handle.c"
    "src/marshal.c"
    "src/server.c"
    "src/socket.c"
    "src/variant.c"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PUBLIC "XCPBUILD")

project("test_s")
add_executable(${PROJECT_NAME} "src/test_s/main.c")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} "xcp")

project("test_c")
add_executable(${PROJECT_NAME} "src/test_c/main.c")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} "xcp")

