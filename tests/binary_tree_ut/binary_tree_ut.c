// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#endif

#include "testrunnerswitcher.h"

static void* my_gballoc_malloc(size_t size)
{
    return malloc(size);
}

static void my_gballoc_free(void* ptr)
{
    free(ptr);
}

static void* my_gballoc_realloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

/*#include "umock_c.h"
#include "umocktypes_charptr.h"
#include "umocktypes_stdint.h"
#include "umock_c_negative_tests.h"
#include "azure_c_shared_utility/macro_utils.h"*/

/*#define ENABLE_MOCKS
#include "azure_c_shared_utility/gballoc.h"
#undef ENABLE_MOCKS*/

#include "binary_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}
#endif

static const NODE_KEY INSERT_FOR_NO_ROTATION[] = { 0xa, 0xb, 0x5, 0x7, 0xc, 0x3 };
static const NODE_KEY INSERT_FOR_RIGHT_ROTATION[] = { 0xa, 0xb, 0x7, 0x5, 0x3 };
static const NODE_KEY INSERT_FOR_LEFT_ROTATION[] = { 0xa, 0xb, 0x7, 0x5, 0x3 };
static const NODE_KEY INVALID_ITEM = 0x01;

static void* DATA_VALUE = (void*)0x11;

//DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

/*static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    char temp_str[256];
    (void)snprintf(temp_str, sizeof(temp_str), "umock_c reported error :%s", ENUM_TO_STRING(UMOCK_C_ERROR_CODE, error_code));
    ASSERT_FAIL(temp_str);
}*/

static TEST_MUTEX_HANDLE g_testByTest;
static TEST_MUTEX_HANDLE g_dllByDll;

BEGIN_TEST_SUITE(binary_tree_ut)

    TEST_SUITE_INITIALIZE(suite_init)
    {
        //int result;
        TEST_INITIALIZE_MEMORY_DEBUG(g_dllByDll);
        g_testByTest = TEST_MUTEX_CREATE();
        ASSERT_IS_NOT_NULL(g_testByTest);
    }

    TEST_SUITE_CLEANUP(suite_cleanup)
    {
        TEST_MUTEX_DESTROY(g_testByTest);
        TEST_DEINITIALIZE_MEMORY_DEBUG(g_dllByDll);
    }

    TEST_FUNCTION_INITIALIZE(method_init)
    {
        if (TEST_MUTEX_ACQUIRE(g_testByTest))
        {
            ASSERT_FAIL("Could not acquire test serialization mutex.");
        }
    }

    TEST_FUNCTION_CLEANUP(method_cleanup)
    {
        TEST_MUTEX_RELEASE(g_testByTest);
    }

    static int should_skip_index(size_t current_index, const size_t skip_array[], size_t length)
    {
        int result = 0;
        for (size_t index = 0; index < length; index++)
        {
            if (current_index == skip_array[index])
            {
                result = __LINE__;
                break;
            }
        }
        return result;
    }

    TEST_FUNCTION(binary_tree_create_succeed)
    {
        //arrange

        //act
        BINARY_TREE_HANDLE result = binary_tree_create();


        //assert
        ASSERT_IS_NOT_NULL(result);

        //cleanup
        binary_tree_destroy(result);
    }

    TEST_FUNCTION(binary_tree_destroy_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE result = binary_tree_create();

        //act
        binary_tree_destroy(result);

        //assert

        //cleanup
    }

    TEST_FUNCTION(binary_tree_destroy_handle_NULL_succeed)
    {
        //arrange

        //act
        binary_tree_destroy(NULL);

        //assert

        //cleanup
    }

    TEST_FUNCTION(binary_tree_insert_handle_NULL_fail)
    {
        //arrange
        NODE_KEY insert_key = 0x4;

        //act
        int result = binary_tree_insert(NULL, insert_key, NULL);

        //assert
        ASSERT_ARE_NOT_EQUAL(int, 0, result);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_insert_succeed)
    {
        //arrange
        NODE_KEY insert_key = 0x4;
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        int result = binary_tree_insert(handle, insert_key, NULL);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_right_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_RIGHT_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_RIGHT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //cleanup
        binary_tree_destroy(handle);
    }
    
    END_TEST_SUITE(binary_tree_ut)
