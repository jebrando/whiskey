// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <vld.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

static const NODE_KEY KEY_VALUES[] = { 0xa, 0xb, 0x5, 0x7, 0xc, 0x3 };
static const NODE_KEY INVALID_ITEM = 0x01;

static void* DATA_VALUE = (void*)0x11;

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
    if (binary_tree_find(handle, item_to_find) == DATA_VALUE)
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
    else
    {
        size_t count = sizeof(KEY_VALUES);
        if (insert_items(handle, KEY_VALUES, count) == 0)
        {
            binary_tree_print_tree(handle);

            // Find a valid item
            if (find_item(handle, KEY_VALUES[1]) != 0)
            {
                (void)printf("FAILURE: Looking for a valid item has failed\r\n");
            }

            // Look for an item not there
            if (find_item(handle, INVALID_ITEM) == 0)
            {
                (void)printf("FAILURE: Found an item that's not there, what is going on?\r\n");
            }

        }
        binary_tree_destroy(handle);
    }
    (void)printf("Press any key to exit:");
    (void)getchar();
}