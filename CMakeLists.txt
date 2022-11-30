cmake_minimum_required(VERSION 3.23)
project(UpdateDemo C)

add_subdirectory( messageV1 )
add_subdirectory( messageV2 )
add_subdirectory( UpdaterClient )
add_subdirectory( UpdaterService )

set(CMAKE_C_STANDARD 17)




