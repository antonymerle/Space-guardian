cmake_minimum_required(VERSION 3.10)
project(SpaceGuardian)

set(CMAKE_C_STANDARD 99)

set(SDL2_INCLUDE_DIRS "/usr/include/SDL2")
set(SDL2_LIBRARIES "/usr/lib/x86_64-linux-gnu")

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARIES})

add_executable(SpaceGuardian main.c background.c draw.c highscore.c init.c input.c sound.c stage.c text.c title.c util.c)
target_link_libraries(SpaceGuardian SDL2 SDL2_image SDL2_mixer)
