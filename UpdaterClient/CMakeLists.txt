cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)
include_directories(../includes)
add_executable(UpdaterClient UpdaterClient.c ClientHandler.c Request.c ../includes/Connection.h)


