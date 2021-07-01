cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
set(PLATFORM_LINUX OFF CACHE BOOL "Linux platform target")
set(PLATFORM_WINDOWS OFF CACHE BOOL "Windows platform target (msys/mingw64)")
set(PLATFORM_SWITCH OFF CACHE BOOL "Nintendo Switch platform target")
set(PLATFORM_3DS OFF CACHE BOOL "Nintendo 3DS platform target")
set(PLATFORM_VITA OFF CACHE BOOL "Sony PS Vita platform target")
set(PLATFORM_DREAMCAST OFF CACHE BOOL "Sega Dreamcast platform target")

####################
# BUILD OPTIONS
####################
set(YOULEAD_KEY OFF CACHE STRING "YouLead Leaderboard Key")

###################
# MODULES
###################
add_subdirectory(libcross2d)

##############
# project
##############
project(SillyTower)

# pplay version
set(VERSION_MAJOR "1")
set(VERSION_MINOR "3")
set(PROJECT_AUTHOR "Cpasjuste")
set(TITLE_ID "STOWE0001")

set(STOWE_INC src src/ui src/leaderboard)
file(GLOB STOWE_SRC src/*.c* src/ui/*.c* src/leaderboard/*.c*)

set(STOWE_CFLAGS
        -D__YOULEAD_KEY__="${YOULEAD_KEY}"
        -D__VERSION_MAJOR__=${VERSION_MAJOR}
        -D__VERSION_MINOR__=${VERSION_MINOR})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(CURL REQUIRED)
if (NOT PLATFORM_SWITCH AND NOT PLATFORM_3DS)
    find_package(OpenSSL REQUIRED)
endif ()
if (NOT PLATFORM_3DS)
    find_package(SDL2_mixer REQUIRED)
    list(APPEND STOWE_CFLAGS -D__WITH_SDL2_MIXER__)
endif ()
set(STOWE_LDFLAGS ${SDL2_MIXER_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_SWITCH)
    list(APPEND STOWE_LDFLAGS
            vorbisfile vorbis ogg opusfile opus modplug mpg123
            mbedtls mbedcrypto mbedx509)
elseif (PLATFORM_3DS)
    list(APPEND STOWE_LDFLAGS
            mbedtls mbedcrypto mbedx509)
elseif (PLATFORM_VITA)
    list(APPEND STOWE_LDFLAGS
            vorbisfile vorbis ogg mikmod FLAC mpg123
            SceNet_stub SceNetCtl_stub)
elseif (PLATFORM_LINUX)
endif ()

#####################
# executable
#####################
add_executable(${CMAKE_PROJECT_NAME} ${STOWE_SRC} src/music.cpp src/music.h)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${STOWE_INC})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${STOWE_CFLAGS})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_link_libraries(${CMAKE_PROJECT_NAME} cross2d ${STOWE_LDFLAGS})

#####################
# targets
#####################
include(libcross2d/cmake/targets.cmake)
