#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 2.8.11)

#compileAsC11()
set(theseTestsName binary_tree_ut)

set(${theseTestsName}_test_files
    ${theseTestsName}.c
)

set(${theseTestsName}_c_files
    ../../binary_tree.c
)

set(${theseTestsName}_h_files
)

include_directories(${TESTRUNNERSWITCHER_INC_FOLDER} ${CTEST_INCLUDES})

function(c_windows_unittests_add_dll whatIsBuilding folder)
    link_directories(${whatIsBuilding}_dll $ENV{VCInstallDir}UnitTest/lib)

    add_library(${whatIsBuilding}_testsonly_lib STATIC ${${whatIsBuilding}_test_files})
    SET(VAR 1)
    foreach(file ${${whatIsBuilding}_test_files})
        set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS -DCPPUNITTEST_SYMBOL=some_symbol_for_cppunittest_${VAR})
        MATH(EXPR VAR "${VAR}+1")
    endforeach()

    set_target_properties(${whatIsBuilding}_testsonly_lib PROPERTIES FOLDER ${folder} )

    target_include_directories(${whatIsBuilding}_testsonly_lib PUBLIC ${sharedutil_include_directories} $ENV{VCInstallDir}UnitTest/include)
    target_compile_definitions(${whatIsBuilding}_testsonly_lib PUBLIC -DCPP_UNITTEST)
    target_compile_options(${whatIsBuilding}_testsonly_lib PUBLIC /TP /EHsc)

    add_library(${whatIsBuilding}_dll SHARED
        ${${whatIsBuilding}_cpp_files}
        ${${whatIsBuilding}_h_files}
        ${${whatIsBuilding}_c_files}
    )

    set_target_properties(${whatIsBuilding}_dll PROPERTIES FOLDER ${folder})

    #set_source_files_properties(${${whatIsBuilding}_c_files} ${logging_files} PROPERTIES COMPILE_FLAGS /TC)
    set_source_files_properties(${${whatIsBuilding}_cpp_files} PROPERTIES COMPILE_FLAGS /TP)

    target_link_libraries(${whatIsBuilding}_dll testrunnerswitcher ${whatIsBuilding}_testsonly_lib )

    set(PARSING_ADDITIONAL_LIBS OFF)
    set(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
    set(VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER)
    foreach(f ${ARGN})
        set(skip_to_next FALSE)
        if(${f} STREQUAL "ADDITIONAL_LIBS")
            SET(PARSING_ADDITIONAL_LIBS ON)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
            #also unset all the other states
            set(skip_to_next TRUE)
        elseif(${f} STREQUAL "VALGRIND_SUPPRESSIONS_FILE")
            SET(PARSING_ADDITIONAL_LIBS OFF)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE ON)
            set(skip_to_next TRUE)
        endif()

        if(NOT skip_to_next)
            if(PARSING_ADDITIONAL_LIBS)
                target_link_libraries(${whatIsBuilding}_dll ${f})
            endif()
        endif()

    endforeach()

    SET(SPACES " ")
    SET(VAR 1)
    foreach(file ${${whatIsBuilding}_test_files})
        # for x64 the underscore is not needed
        if (ARCHITECTURE STREQUAL "x86_64" OR ARCHITECTURE STREQUAL "ARM")
            set_property(TARGET ${whatIsBuilding}_dll APPEND_STRING PROPERTY LINK_FLAGS ${SPACES}/INCLUDE:"some_symbol_for_cppunittest_${VAR}")
        else()
            set_property(TARGET ${whatIsBuilding}_dll APPEND_STRING PROPERTY LINK_FLAGS ${SPACES}/INCLUDE:"_some_symbol_for_cppunittest_${VAR}")
        endif()
        MATH(EXPR VAR "${VAR}+1")
    endforeach()
endfunction()

function(c_windows_unittests_add_exe whatIsBuilding folder)
    include_directories(${TESTRUNNERSWITCHER_INC_FOLDER} ${CTEST_INCLUDES})

    add_executable(${whatIsBuilding}_exe
        ${${whatIsBuilding}_test_files}
        ${${whatIsBuilding}_cpp_files}
        ${${whatIsBuilding}_h_files}
        ${${whatIsBuilding}_c_files}
        ${CMAKE_CURRENT_LIST_DIR}/main.c
    )
    set_target_properties(${whatIsBuilding}_exe PROPERTIES FOLDER ${folder})

    target_compile_definitions(${whatIsBuilding}_exe PUBLIC -DUSE_CTEST)
    target_include_directories(${whatIsBuilding}_exe PUBLIC ${sharedutil_include_directories})
    target_link_libraries(${whatIsBuilding}_exe ctest testrunnerswitcher)

    set(PARSING_ADDITIONAL_LIBS OFF)
    set(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
    set(VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER)
    foreach(f ${ARGN})
        set(skip_to_next FALSE)
        if(${f} STREQUAL "ADDITIONAL_LIBS")
            SET(PARSING_ADDITIONAL_LIBS ON)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
            #also unset all the other states
            set(skip_to_next TRUE)
        elseif(${f} STREQUAL "VALGRIND_SUPPRESSIONS_FILE")
            SET(PARSING_ADDITIONAL_LIBS OFF)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE ON)
            set(skip_to_next TRUE)
        endif()

        if(NOT skip_to_next)
            if(PARSING_ADDITIONAL_LIBS)
                target_link_libraries(${whatIsBuilding}_exe ${f})
            endif()
        endif()
    endforeach()

    add_test(NAME ${whatIsBuilding} COMMAND ${whatIsBuilding}_exe)
endfunction()

#this function takes more than the 1 mandatory argument (whatIsBuilding)
#the parameters are separated by "known" separators
#for example, ADDITIONAL_LIBS starts a list of needed libraries
function(c_linux_unittests_add_exe whatIsBuilding folder)

    add_executable(${whatIsBuilding}_exe
        ${${whatIsBuilding}_test_files}
        ${${whatIsBuilding}_cpp_files}
        ${${whatIsBuilding}_h_files}
        ${${whatIsBuilding}_c_files}
        ${CMAKE_CURRENT_LIST_DIR}/main.c
    )
    set_target_properties(${whatIsBuilding}_exe PROPERTIES FOLDER ${folder})

    target_compile_definitions(${whatIsBuilding}_exe PUBLIC -DUSE_CTEST)
    target_include_directories(${whatIsBuilding}_exe PUBLIC ${sharedutil_include_directories})

    #this part detects
    #       - the additional libraries that might be needed.
    #     additional libraries are started by ADDITIONAL_LIBS parameter and ended by any other known parameter (or end of variable arguments)
    #   - a valgrind suppression file (VALGRIND_SUPPRESSIONS_FILE) for memcheck
    #     the file name follows immediately after

    set(PARSING_ADDITIONAL_LIBS OFF)
    set(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
    set(VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER)
    foreach(f ${ARGN})
        set(skip_to_next FALSE)
        if(${f} STREQUAL "ADDITIONAL_LIBS")
            SET(PARSING_ADDITIONAL_LIBS ON)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE OFF)
            #also unset all the other states
            set(skip_to_next TRUE)
        elseif(${f} STREQUAL "VALGRIND_SUPPRESSIONS_FILE")
            SET(PARSING_ADDITIONAL_LIBS OFF)
            SET(PARSING_VALGRIND_SUPPRESSIONS_FILE ON)
            set(skip_to_next TRUE)
        endif()

        if(NOT skip_to_next)
            if(PARSING_ADDITIONAL_LIBS)
                target_link_libraries(${whatIsBuilding}_exe ${f})
            elseif(PARSING_VALGRIND_SUPPRESSIONS_FILE)
                set(VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER "--suppressions=${f}")
            endif()
        endif()
    endforeach()

    target_link_libraries(${whatIsBuilding}_exe ctest m)

    add_test(NAME ${whatIsBuilding} COMMAND $<TARGET_FILE:${whatIsBuilding}_exe>)

    if(${run_valgrind})
        find_program(VALGRIND_FOUND NAMES valgrind)
        if(${VALGRIND_FOUND} STREQUAL VALGRIND_FOUND-NOTFOUND)
            message(WARNING "run_valgrind was TRUE, but valgrind was not found - there will be no tests run under valgrind")
        else()
            add_test(NAME ${whatIsBuilding}_valgrind COMMAND valgrind                 --gen-suppressions=all --num-callers=100 --error-exitcode=1 --leak-check=full --track-origins=yes ${VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER} $<TARGET_FILE:${whatIsBuilding}_exe>)
            add_test(NAME ${whatIsBuilding}_helgrind COMMAND valgrind --tool=helgrind --gen-suppressions=all --num-callers=100 --error-exitcode=1 ${VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER} $<TARGET_FILE:${whatIsBuilding}_exe>)
            add_test(NAME ${whatIsBuilding}_drd      COMMAND valgrind --tool=drd      --gen-suppressions=all --num-callers=100 --error-exitcode=1 ${VALGRIND_SUPPRESSIONS_FILE_EXTRA_PARAMETER} $<TARGET_FILE:${whatIsBuilding}_exe>)
        endif()
    endif()
endfunction()

function(build_c_test_artifacts whatIsBuilding use_gballoc folder)

    #the first argument is what is building
    #the second argument is whether the tests should be build with gballoc #defines or not
    #the following arguments are a list of libraries to link with

    #setting #defines
    if(WIN32)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    else()
    endif()

    #setting includes
    set(sharedutil_include_directories ${sharedutil_include_directories} ${TESTRUNNERSWITCHER_INCLUDES} ${CTEST_INCLUDES})

    if(WIN32)
    else()
        include_directories(${sharedutil_include_directories})
    endif()

    #setting output type
    if(WIN32)
            #if (${use_cppunittest})
                c_windows_unittests_add_dll(${whatIsBuilding} ${folder} ${ARGN})
            #endif()
            c_windows_unittests_add_exe(${whatIsBuilding} ${folder} ${ARGN})
    else()
            c_linux_unittests_add_exe(${whatIsBuilding} ${folder} ${ARGN})
    endif()
endfunction()

build_c_test_artifacts(${theseTestsName} ON "tests")
