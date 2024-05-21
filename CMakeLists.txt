cmake_minimum_required(VERSION 3.22)

project(sentry_map_generator)

find_package(OpenCV 4.5.4 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})