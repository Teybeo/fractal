cmake_minimum_required(VERSION 3.12)
project(fractol C)

add_compile_options(-Wall -Wextra)
#add_compile_options(-Wall -Wextra)

#link_libraries(-fsanitize=address)

set(CMAKE_C_FLAGS_RELEASE "-O2 -flto -g3 -march=native")

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        app.c
        t_float2.c)

add_executable(fractol ${SOURCE_FILES})
target_link_libraries(fractol mlx)
target_link_libraries(fractol "-framework OpenGL -framework AppKit")
