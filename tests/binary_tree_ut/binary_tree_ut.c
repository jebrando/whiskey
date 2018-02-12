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

#include <windows.h>

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

static void remove_callback(void* data)
{
    //DATA_VALUE
    (void)data;
}

#include "binary_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}
#endif

static const NODE_KEY BASIC_INSERT_FOR_NODES[] = { 0xa, 0xc, 0xe };
static const NODE_KEY BASIC_INSERT_2_FOR_NODES[] = { 14, 12, 10 };
static const char* VISUAL_BASIC_INSERT = "12(*10)(*17)";

static const NODE_KEY COMPLEX_INSERT_FOR_NODES[] = { 0x8, 18, 5, 0xf, 17, 25, 40, 80 };
static const char* VISUAL_COMPLEX_INSERT = "17(*8(5)(15))(*25(18)(40(*80)))";

static const size_t INSERT_NO_ROTATION_HEIGHT = 3;
static const NODE_KEY INSERT_FOR_NO_ROTATION[] = { 0xa, 0xb, 0x5, 0x7, 0xc, 0x3 };
static const char* VISUAL_NO_ROTATION = "10(5(*3)(*7))(11(*12))";

static const NODE_KEY INSERT_FOR_NO_ROTATION_2[] = { 0xa, 0xc, 0x5, 0x7, 0xb, 0x3 };
static const char* VISUAL_NO_ROTATION_2 = "a(5(3)(7))(c(b))";

static const NODE_KEY INSERT_FOR_RIGHT_ROTATION[] = { 0xa, 0xb, 0x7, 0x5, 0x3 };
static const char* VISUAL_RIGHT_ROTATION = "a(5(3)(7))(b)";

static const NODE_KEY INSERT_FOR_RIGHT_LEFT_ROTATION[] = { 0x10, 0x14, 0xe, 0xa, 0xc };
static const char* VISUAL_RIGHT_LEFT_ROTATION = "10(c(a)(e))(14)";

static const NODE_KEY INSERT_FOR_LEFT_RIGHT_ROTATION[] = { 0xa, 0x6, 0xd, 0x12, 0xe };
static const char* VISUAL_LEFT_RIGHT_ROTATION = "a(6)(e(d)(12))"; 
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

    static void assert_visual_check(BINARY_TREE_HANDLE handle, const char* expected)
    {
        char* visual_check = binary_tree_construct_visual(handle);
        ASSERT_ARE_EQUAL(char_ptr, expected, visual_check);
        free(visual_check);
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

    TEST_FUNCTION(binary_tree_insert_one_item_succeed)
    {
        //arrange
        NODE_KEY insert_key = 0x4;
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        int result = binary_tree_insert(handle, insert_key, NULL);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        assert_visual_check(handle, "4");


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_basic_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(BASIC_INSERT_FOR_NODES)/sizeof(BASIC_INSERT_FOR_NODES[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, BASIC_INSERT_FOR_NODES[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //assert
        assert_visual_check(handle, VISUAL_BASIC_INSERT);


        //cleanup
        binary_tree_destroy(handle);
    }
#if 0
    TEST_FUNCTION(binary_tree_insert_2_basic_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(BASIC_INSERT_2_FOR_NODES)/ sizeof(BASIC_INSERT_2_FOR_NODES[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, BASIC_INSERT_2_FOR_NODES[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //assert
        assert_visual_check(handle, VISUAL_BASIC_INSERT);


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_no_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //assert
        assert_visual_check(handle, VISUAL_NO_ROTATION);


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_complex_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(COMPLEX_INSERT_FOR_NODES)/sizeof(COMPLEX_INSERT_FOR_NODES[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, COMPLEX_INSERT_FOR_NODES[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //assert
        assert_visual_check(handle, VISUAL_COMPLEX_INSERT);


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_random_failure_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        // Fails on 30
        //static const NODE_KEY INSERT_RANDOM_FAILURE[] = { 5, 44, 83, 30, 63, 23, 67, 100, 37, 98, 75, 66, 49, 61, 82, 47, 69, 19, 31, 76, 87, 86, 21, 16, 89 };//, 45, 97, 62, 7, 27, 79 };//, 60, 14 };
        static const NODE_KEY INSERT_RANDOM_FAILURE[] = { 11, 2, 14, 1, 7, 15, 5, 8, 4 };
        static const char* VISUAL_RANDOM_FAILURE = "7(*2(1)(5(*4)))(*11(8)(14(*15)))";
        //act
        size_t count = sizeof(INSERT_RANDOM_FAILURE) / sizeof(INSERT_RANDOM_FAILURE[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_RANDOM_FAILURE[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }

        //assert
        assert_visual_check(handle, VISUAL_RANDOM_FAILURE);


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_right_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_RIGHT_ROTATION)/ sizeof(INSERT_FOR_RIGHT_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_RIGHT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }
        assert_visual_check(handle, VISUAL_RIGHT_ROTATION);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_right_left_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_RIGHT_LEFT_ROTATION)/sizeof(INSERT_FOR_RIGHT_LEFT_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_RIGHT_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }
        assert_visual_check(handle, VISUAL_RIGHT_LEFT_ROTATION);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_right_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        size_t count = sizeof(INSERT_FOR_LEFT_RIGHT_ROTATION)/sizeof(INSERT_FOR_LEFT_RIGHT_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_RIGHT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }
        assert_visual_check(handle, VISUAL_LEFT_RIGHT_ROTATION);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_rotate_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        const NODE_KEY INSERT_FOR_LEFT_ROTATION[] = { 0x7, 0x5, 0xa, 0xb, 0xd };
        const char* VISUAL_LEFT_ROTATION = "7(5)(b(a)(d))";

        size_t count = sizeof(INSERT_FOR_LEFT_ROTATION)/sizeof(INSERT_FOR_LEFT_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }
        assert_visual_check(handle, VISUAL_LEFT_ROTATION);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_insert_left_rotate_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        static const NODE_KEY INSERT_FOR_LEFT_ROTATION[] = { 0x7, 0x5, 0xa, 0xd, 0xb };
        const char* VISUAL_LEFT_ROTATION = "7(5)(b(a)(d))";

        size_t count = sizeof(INSERT_FOR_LEFT_ROTATION)/sizeof(INSERT_FOR_LEFT_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            int result = binary_tree_insert(handle, INSERT_FOR_LEFT_ROTATION[index], DATA_VALUE);

            //assert
            ASSERT_ARE_EQUAL(int, 0, result);
        }
        assert_visual_check(handle, VISUAL_LEFT_ROTATION);

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
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
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
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
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
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
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
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
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
        size_t count = sizeof(INSERT_FOR_NO_ROTATION)/sizeof(INSERT_FOR_NO_ROTATION[0]);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        size_t item_height = binary_tree_height(handle);

        //assert
        ASSERT_ARE_EQUAL(size_t, INSERT_NO_ROTATION_HEIGHT, item_height);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_handle_NULL_fail)
    {
        //arrange

        //act
        int result = binary_tree_remove(NULL, INSERT_FOR_NO_ROTATION[0], remove_callback);

        //assert
        ASSERT_ARE_NOT_EQUAL(int, 0, result);

        //cleanup
    }
#if 0
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
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[2], remove_callback);

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
        int result = binary_tree_remove(handle, REMOVE_TWO_CHILDREN_2[1], remove_callback);

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
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[1], remove_callback);

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
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION_2[1], remove_callback);

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
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        const char* VISUAL_NO_ROTATION_AFTER_REMOVE = "a(5(7))(b(c))";

        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[count-1], remove_callback);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        assert_visual_check(handle, VISUAL_NO_ROTATION_AFTER_REMOVE);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_node_no_children_succeed)
    {
        //arrange 
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        const char* VISUAL_NO_ROTATION_AFTER_REMOVE = "a(5(3))(b(c))";

        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[3], remove_callback);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);
        assert_visual_check(handle, VISUAL_NO_ROTATION_AFTER_REMOVE);


        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_root_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        static const char* VISUAL_NO_ROTATION_AFTER_REMOVE = "b(5(3)(7))(c)";

        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[0], remove_callback);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        assert_visual_check(handle, VISUAL_NO_ROTATION_AFTER_REMOVE);

        //cleanup
        binary_tree_destroy(handle);
    }

    TEST_FUNCTION(binary_tree_remove_root_2_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();
        size_t count = sizeof(INSERT_FOR_NO_ROTATION);
        static const char* VISUAL_NO_ROTATION_AFTER_REMOVE = "b(5(3)(7))(c)";

        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, INSERT_FOR_NO_ROTATION[index], DATA_VALUE);
        }

        //act
        int result = binary_tree_remove(handle, INSERT_FOR_NO_ROTATION[0], remove_callback);

        //assert
        ASSERT_ARE_EQUAL(int, 0, result);

        assert_visual_check(handle, VISUAL_NO_ROTATION_AFTER_REMOVE);

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
        int result = binary_tree_remove(handle, INVALID_ITEM, remove_callback);

        //assert
        ASSERT_ARE_NOT_EQUAL(int, 0, result);

        //cleanup
        binary_tree_destroy(handle);
    }
#endif

    TEST_FUNCTION(binary_tree_construct_visual_handle_NULL_fail)
    {
        //arrange
        //act
        char* result = binary_tree_construct_visual(NULL);

        //assert
        ASSERT_IS_NULL(result);

        //cleanup
    }

    TEST_FUNCTION(binary_tree_construct_visual_no_items_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        //act
        char* result = binary_tree_construct_visual(handle);

        //assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, "", result);

        //cleanup
        free(result);
        binary_tree_destroy(handle);
    }

     
    TEST_FUNCTION(binary_tree_construct_visual_succeed)
    {
        //arrange
        BINARY_TREE_HANDLE handle = binary_tree_create();

        const NODE_KEY VISUAL_NODES[] = { 10, 5, 15, 12, 7, 2 };
        const char* VISUAL_NODE_VALUE = "a(5(2)(7))(f(c))";

        size_t count = sizeof(VISUAL_NODES)/sizeof(VISUAL_NODES[0]);
        for (size_t index = 0; index < count; index++)
        {
            (void)binary_tree_insert(handle, VISUAL_NODES[index], DATA_VALUE);
        }

        //act
        char* result = binary_tree_construct_visual(handle);

        //assert
        ASSERT_IS_NOT_NULL(result);
        ASSERT_ARE_EQUAL(char_ptr, VISUAL_NODE_VALUE, result);

        //cleanup
        free(result);
        binary_tree_destroy(handle);
    }
#endif
    END_TEST_SUITE(binary_tree_ut)
