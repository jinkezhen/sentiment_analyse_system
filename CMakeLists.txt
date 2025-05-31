cmake_minimum_required(VERSION 3.5)
project(new_project CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_PREFIX_PATH "/home/jkz/project/new_sentiment_analyse_system/libtorch")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# mysql头文件路径
include_directories(/usr/include/mysql)
link_directories(/usr/lib/x86_64-linux-gnu)

# 先找库，保证下面能用
find_package(Torch REQUIRED)
find_package(Drogon CONFIG REQUIRED)

add_compile_options(
    -march=native
    -mavx512f
    -mavx512bw
    -mavx512vl
    -O3
    -Wextra
    -Wpedantic
)

# 声明 torch_infer 静态库或动态库
add_library(torch_infer
    torch_infer/torch_infer.cc
)

target_include_directories(torch_infer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/torch_infer
    ${TORCH_INCLUDE_DIRS}            # 关键：加上torch头文件路径
)

target_link_libraries(torch_infer
    PRIVATE
    ${TORCH_LIBRARIES}
)

add_executable(${PROJECT_NAME} main.cc)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models
                                   ${CMAKE_CURRENT_SOURCE_DIR}/torch_infer
                                   ${CMAKE_CURRENT_SOURCE_DIR}/controllers
                                    )

# 连接主程序和依赖
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Drogon::Drogon
    torch_infer
    ${TORCH_LIBRARIES}
    mysqlclient
)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(torch_infer TORCH_INFER_SRC)

target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC}
               ${TORCH_INFER_SRC})

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

add_subdirectory(test)
