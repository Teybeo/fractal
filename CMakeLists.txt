cmake_minimum_required(VERSION 3.12)
project(fractol C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        app.c
        )

add_executable(fractol ${SOURCE_FILES})
target_link_libraries(fractol mlx)
target_link_libraries(fractol "-framework OpenGL -framework AppKit")
