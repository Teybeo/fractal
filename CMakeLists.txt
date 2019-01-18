cmake_minimum_required(VERSION 3.12)
project(fractol C)

add_compile_options(-Wall -Wextra)
#add_compile_options(-Wall -Wextra -fsanitize=address -g3)
#
#link_libraries(-fsanitize=address -g3)

set(CMAKE_C_FLAGS_DEBUG "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE "-O2 -ffast-math -flto -march=native")

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        app.c
        t_float2.c
        drawing.c
        config.c
        coloring.c)

add_executable(fractol ${SOURCE_FILES})

add_custom_command(
        TARGET fractol
        PRE_LINK
        COMMAND sh patch_mlx.sh
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
        COMMENT "Patching mlx")


target_link_libraries(fractol "-L ${CMAKE_HOME_DIRECTORY} -l mlx")
target_link_libraries(fractol "-framework OpenGL -framework AppKit")
