cmake_minimum_required(VERSION 3.12)
project(fractol C)

add_compile_options(-Wall -Wextra -g3)

#add_compile_options(-fsanitize=address -g3)
#link_libraries(-fsanitize=address -g3)

#add_compile_options(-fsanitize=thread -g3)
#link_libraries(-fsanitize=thread -g3)

#set(CMAKE_C_FLAGS_DEBUG "-g3")
set(CMAKE_C_FLAGS_DEBUG "-Og")
set(CMAKE_C_FLAGS_RELEASE "-O3 -ffast-math -flto -march=native")

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        app.c
        t_float2.c
        drawing.c
        config.c
        coloring.c
        thread_pool.c)

add_executable(fractol ${SOURCE_FILES})

if (USE_MLX)
    add_custom_command(
            TARGET fractol
            PRE_LINK
            COMMAND sh patch_mlx.sh
            WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
            COMMENT "Patching mlx")
    target_link_directories(fractol PRIVATE ${CMAKE_HOME_DIRECTORY})
    target_link_libraries(fractol mlx)
    target_link_libraries(fractol "-framework OpenGL -framework AppKit")
else()

    set(MLX_SHIM_PATH "D:/Dev/Projets/42/mlx_shim")

    target_include_directories(fractol PUBLIC ${MLX_SHIM_PATH}/include)
    target_link_directories(fractol PRIVATE ${MLX_SHIM_PATH}/lib)
    target_link_libraries(fractol mlx_shim)

    # Add SDL2 path to list of searched paths for packages
    set(CMAKE_PREFIX_PATH "D:/Dev/Lib/SDL2-2.0.8")

    find_package(SDL2 REQUIRED)

    # Stay compatible with old style find_package results
    if (NOT TARGET SDL2::SDL2)
        target_link_libraries(fractol ${SDL2_LIBRARIES})
        target_include_directories(fractol PRIVATE ${SDL2_INCLUDE_DIRS})
    else()
        target_link_libraries(fractol SDL2::SDL2)
    endif()
endif()
