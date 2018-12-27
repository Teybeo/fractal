cmake_minimum_required(VERSION 3.12)
project(fractol C)

add_compile_options(-Wall -Wextra)
#add_compile_options(-Wall -Wextra -fsanitize=address -g3)

#link_libraries(-fsanitize=address -g3)

#set(CMAKE_C_FLAGS_RELEASE "-O2 -ffast-math -flto -march=native")

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        main.c
        app.c
        t_float2.c)

add_executable(fractol ${SOURCE_FILES})

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


#target_link_libraries(fractol "-framework OpenGL -framework AppKit")
