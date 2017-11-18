// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

static const char* STRING_TO_STORE = "CDAB";
static const char INVALID_ITEM = '@';

int insert_items(BINARY_TREE_HANDLE handle)
{
    int result = 0;
    size_t count = strlen(STRING_TO_STORE);
    for (size_t index = 0; index < count; index++)
    {
        if (binary_tree_insert(handle, STRING_TO_STORE[index]) != 0)
        {
            (void)printf("FAILURE: Inserting item %d\r\n", (int)index);
            result = __LINE__;
            break;
        }
    }
    return result;
}

int find_item(BINARY_TREE_HANDLE handle, char item_to_find)
{
    int result;
    if (binary_tree_find(handle, item_to_find) == 0)
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
        if (insert_items(handle) == 0)
        {
            // Find a valid item
            if (find_item(handle, STRING_TO_STORE[1]) != 0)
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
}