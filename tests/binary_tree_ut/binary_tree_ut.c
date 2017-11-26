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

#include "binary_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}
#endif

static const NODE_KEY INSERT_FOR_NO_ROTATION[] = { 0xa, 0xb, 0x5, 0x7, 0xc, 0x3 };
static const NODE_KEY INSERT_FOR_NO_ROTATION_2[] = { 0xa, 0xc, 0x5, 0x7, 0xb, 0x3 };
static const size_t INSERT_NO_ROTATION_HEIGHT = 3;
static const NODE_KEY INSERT_FOR_RIGHT_ROTATION[] = { 0xa, 0xb, 0x7, 0x5, 0x3 };

static const NODE_KEY INSERT_FOR_RIGHT_LEFT_ROTATION[] = { 0x10, 0x14, 0xe, 0xa, 0xc };
static const NODE_KEY INSERT_FOR_LEFT_RIGHT_ROTATION[] = { 0xa, 0x6, 0xd, 0x12, 0xe };
static const NODE_KEY INVALID_ITEM = 0x01;

static void* DATA_VALUE = (void*)0x11;

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

    TEST_FUNCTION(binary_tree_insert_right_left_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_RIGHT_LEFT_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_RIGHT_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_right_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_LEFT_RIGHT_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_RIGHT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        const NODE_KEY INSERT_FOR_LEFT_ROTATION[] = { 0x7, 0x5, 0xa, 0xb, 0xd };

        size_t count = sizeof(INSERT_FOR_LEFT_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_rotate_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        static const NODE_KEY INSERT_FOR_LEFT_ROTATION[] = { 0x7, 0x5, 0xa, 0xd, 0xb };
        size_t count = sizeof(INSERT_FOR_LEFT_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //cleanup
        binary_tree_destroy(handle);
    }


    TEST_FUNCTION(binary_tree_find_handle_NULL_fail)
    {
        //arrange

        //act
        void* found_item = binary_tree_find(NULL, INSERT_FOR_NO_ROTATION[0]);

        ASSERT_IS_NULL(found_item);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_find_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        void* found_item = binary_tree_find(handle, INSERT_FOR_NO_ROTATION[count-1]);

        //assert
        ASSERT_IS_NOT_NULL(found_item);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_find_no_items_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        void* found_item = binary_tree_find(handle, INSERT_FOR_NO_ROTATION[0]);

        //assert
        ASSERT_IS_NULL(found_item);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_find_invalid_item_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        void* found_item = binary_tree_find(handle, INVALID_ITEM);

        //assert
        ASSERT_IS_NULL(found_item);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_find_invalid_item_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        void* found_item = binary_tree_find(handle, 0x20);

        //assert
        ASSERT_IS_NULL(found_item);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_item_count_handle_NULL_fail)
    {
        //arrange

        //act
        size_t item_count = binary_tree_item_count(NULL);

        //assert
        ASSERT_ARE_NOT_EQUAL(size_t, 0, item_count);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_item_count_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        size_t item_count = binary_tree_item_count(handle);

        //assert
        ASSERT_ARE_EQUAL(size_t, count, item_count);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_height_handle_NULL_fail)
    {
        //arrange

        //act
        size_t item_count = binary_tree_height(NULL);

        //assert
        ASSERT_ARE_NOT_EQUAL(size_t, 0, item_count);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_height_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        size_t item_count = binary_tree_height(handle);

        //assert
        ASSERT_ARE_EQUAL(size_t, INSERT_NO_ROTATION_HEIGHT, item_count);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_handle_NULL_fail)
    {
        //arrange

        //act
        int result = binary_tree_remove(NULL, INSERT_FOR_NO_ROTATION[0]);

        //assert
        ASSERT_ARE_NOT_EQUAL(int, 0, result);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_remove_two_children_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[2]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_two_children_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        const NODE_KEY REMOVE_TWO_CHILDREN_2[] = { 0xa, 0xf, 0x6, 0x3, 0xc, 0x12, 0x10 };

        size_t count = sizeof(REMOVE_TWO_CHILDREN_2);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, REMOVE_TWO_CHILDREN_2[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, REMOVE_TWO_CHILDREN_2[1]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_one_child_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[1]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_one_child_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION_2[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION_2[1]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_node_no_children_2_succeed)
    {
        //arrange
        /*BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
        (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[count-1]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);*/
    }

    TEST_FUNCTION(binary_tree_remove_node_no_children_succeed)
    {
        //arrange
        /*BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[count-1]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);*/
    }

    TEST_FUNCTION(binary_tree_remove_root_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[0]);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        // Use as verification since it touches every node
        binary_tree_print(handle);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_item_not_found_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INVALID_ITEM);

        //assert
        ASSERT_ARE_NOT_EQUAL(int, 0, result);

        //cleanup
        binary_tree_destroy(handle);
    }

    END_TEST_SUITE(binary_tree_ut)
