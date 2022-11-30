cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)
include_directories(../includes)
add_library(messageV1 SHARED ${LIB_ABI_SOURCES} MessageV1.c)

