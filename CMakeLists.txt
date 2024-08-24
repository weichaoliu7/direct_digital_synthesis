cmake_minimum_required(VERSION  3.19.2)
project(direct_digital_synthesis C)

set(CMAKE_C_STANDARD 11 "-static-libgcc -static-libstdc++")

# add_executable(logic_test logic_test.c logic_block.c)
add_executable(dds main.c logic_block.c nco.c)