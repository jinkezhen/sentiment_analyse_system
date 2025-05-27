cmake_minimum_required(VERSION 3.5)
project(new_project_test CXX)

# 移除：find_package(Drogon CONFIG REQUIRED)

find_package(Torch REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../torch_infer
)

add_executable(${PROJECT_NAME} test_main.cc)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Drogon::Drogon   # 注意：这里只链接，不重复查找
    ${TORCH_LIBRARIES}
    torch_infer
)

ParseAndAddDrogonTests(${PROJECT_NAME})
