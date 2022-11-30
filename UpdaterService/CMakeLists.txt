cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)
include_directories(../includes)
add_executable(UpdaterService UpdaterService.c ../includes/Handler.h)