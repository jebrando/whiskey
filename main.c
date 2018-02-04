// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <vld.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "binary_tree.h"
#include "stopwatch.h"

#define ITEMS_COUNT    100

// Right Rotation
static const NODE_KEY KEY_VALUES[] = { 0xa, 0xf, 0x5, 0xb, 0x7, 0x3, 0x12 };

static const NODE_KEY INVALID_ITEM = 0x01;

static void* DATA_VALUE = (void*)0x11;

STOPWATCH_HANDLE g_timer_handle;

static int large_insert(BINARY_TREE_HANDLE handle)
{
    int result = 0;

    NODE_KEY insert_list[ITEMS_COUNT];
    printf("Loading Array with %d items ...\r\n", ITEMS_COUNT);
    stopwatch_start(g_timer_handle);
    for (NODE_KEY index = 0; index < ITEMS_COUNT; index++)
    {
        insert_list[index] = index+1;
    }
    double time_elapsed = stopwatch_get_elapsed(g_timer_handle);
    (void)printf("Loading the array took %f seconds\r\n", time_elapsed);


    printf("Shuffle the Array ...\r\n");
    srand((unsigned int)time(NULL));
    stopwatch_reset(g_timer_handle);
    for (NODE_KEY index = 0; index < ITEMS_COUNT; index++)
    {
        NODE_KEY pos = rand() % (index+1);
        insert_list[index] = index + 1;

        // Swap the Items
        NODE_KEY temp = insert_list[index];
        insert_list[index] = insert_list[pos];
        insert_list[pos] = temp;
    }
    time_elapsed = stopwatch_get_elapsed(g_timer_handle);
    (void)printf("Shuffling the array took %f seconds\r\n", time_elapsed);
    stopwatch_reset(g_timer_handle);


    printf("Inserting the Array ...\r\n");
    stopwatch_reset(g_timer_handle);
    for (NODE_KEY index = 0; index < ITEMS_COUNT; index++)
    {
        if (binary_tree_insert(handle, insert_list[index], DATA_VALUE) != 0)
        {
            (void)printf("FAILURE: Inserting item %d\r\n", (int)index);
            result = __LINE__;
            break;
        }
    }
    time_elapsed = stopwatch_get_elapsed(g_timer_handle);
    (void)printf("Inserting the array took %f seconds\r\n", time_elapsed);
    stopwatch_reset(g_timer_handle);
    return result;
}

int insert_items(BINARY_TREE_HANDLE handle, const NODE_KEY insert_group[], size_t count)
{
    int result = 0;

    for (size_t index = 0; index < count; index++)
    {
        if (binary_tree_insert(handle, insert_group[index], DATA_VALUE) != 0)
        {
            (void)printf("FAILURE: Inserting item %d\r\n", (int)index);
            result = __LINE__;
            break;
        }
    }
    (void)printf(" Tree Items %d\r\n", (int)binary_tree_item_count(handle) );
    (void)printf("Tree Height %d\r\n", (int)binary_tree_height(handle));
    return result;
}

int find_item(BINARY_TREE_HANDLE handle, char item_to_find)
{
    int result;
    stopwatch_start(g_timer_handle);
    void* find_item = binary_tree_find(handle, item_to_find);
    double time_elapsed = stopwatch_get_elapsed(g_timer_handle);
    (void)printf("Seconds Elapsed %f\r\n", time_elapsed);

    if (find_item == DATA_VALUE)
    {
        (void)printf("The item has been found\r\n");
        result = 0;
    }
    else
    {

        (void)printf("The item was not found\r\n");
        result = 1;
    }
    return result;
}

int main(void)
{
    BINARY_TREE_HANDLE handle = binary_tree_create();
    if (handle == NULL)
    {
        (void)printf("FAILURE: creating binary tree\r\n");
    }
    else if ((g_timer_handle = stopwatch_create()) == NULL)
    {
        binary_tree_destroy(handle);
        (void)printf("FAILURE: creating stopwatch\r\n");
    }
    else
    {
        //size_t count = sizeof(KEY_VALUES);
        //if (insert_items(handle, KEY_VALUES, count) == 0)
         if (large_insert(handle))
        {

            // Find a valid item
            /*if (find_item(handle, KEY_VALUES[1]) != 0)
            {
                (void)printf("FAILURE: Looking for a valid item has failed\r\n");
            }

            // Look for an item not there
            if (find_item(handle, INVALID_ITEM) == 0)
            {
                (void)printf("FAILURE: Found an item that's not there, what is going on?\r\n");
            }*/

        }
        binary_tree_destroy(handle);
        stopwatch_destroy(g_timer_handle);
    }
    (void)printf("Press any key to exit:");
    (void)getchar();
}