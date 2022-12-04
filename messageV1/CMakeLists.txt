cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)
include_directories(../includes)
add_library(messageV1 SHARED ${LIB_ABI_SOURCES} MessageV1.c ../includes/ServerHandler.h)

#add_test(NAME Base_Addr_Check COMMAND "{objdump libmessageV1.so -T | awk '$0 ~ /.*getVersionMessage.*/ {printf "base address:%s\n",$1 }'}")
