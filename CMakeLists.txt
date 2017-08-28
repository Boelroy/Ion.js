cmake_minimum_required(VERSION 3.9)

PROJECT(Ion)

INCLUDE_DIRECTORIES(
    src
)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
ADD_EXECUTABLE(ion src/ion.cc)