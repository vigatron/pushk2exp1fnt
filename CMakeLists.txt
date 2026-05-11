cmake_minimum_required(VERSION 3.16)

project(pushk2exp1fnt LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/mods
                    ${CMAKE_SOURCE_DIR}/src/font
                    ${CMAKE_SOURCE_DIR}/src/misc
                    ${CMAKE_SOURCE_DIR}/src/pseudo
                    )

# Опции компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Источники
file(GLOB_RECURSE source_list
    src/*
    src/font/*
	src/mods/*
	src/misc/*
    src/pseudo/*
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${source_list} )

# Установка бинарника
# install(TARGETS ${PROJECT_NAME}
#     RUNTIME DESTINATION bin
# )

# python3 -m unittest discover -s tests
