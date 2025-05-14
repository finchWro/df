cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(df)


# Needed for colorful output
zephyr_compile_options(-fdiagnostics-color=always)

target_sources(app PRIVATE src/main.c)