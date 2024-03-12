cmake_minimum_required(VERSION 3.16)
project(video_info)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
        libavdevice
        libavfilter
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
        )

add_executable(${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
        PkgConfig::LIBAV
        )

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)