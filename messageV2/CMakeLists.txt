cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)
include_directories(../includes)
add_library(messageV2 SHARED ${LIB_ABI_SOURCES} MessageV2.c)


