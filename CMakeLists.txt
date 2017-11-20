#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for iothub_client_sample_amqp
cmake_minimum_required(VERSION 2.8.11)
project(whiskey)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF(WIN32)
    #windows needs this define
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DGB_MEASURE_MEMORY_FOR_THIS -DGB_DEBUG_ALLOC)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4232")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /wd4232")
        # Make warning as error
    add_definitions(/WX)
ELSE()
    # Make warning as error
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif(WIN32)

set(whiskey_c_files
    binary_tree.c
    main.c
)

set(whiskey_h_files
    binary_tree.h
)

#Conditionally use the SDK trusted certs in the samples

include_directories(.)

add_executable(binary_tree ${whiskey_c_files} ${whiskey_h_files})

set(CTEST_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/deps/ctest/inc)

add_subdirectory(./deps/testrunnerswitcher)

add_subdirectory(./deps/ctest)

add_subdirectory(tests)
