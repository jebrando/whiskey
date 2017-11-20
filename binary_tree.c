// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

typedef enum LOG_CATEGORY_TAG
{
    AZ_LOG_ERROR,
    AZ_LOG_INFO,
    AZ_LOG_TRACE
} LOG_CATEGORY;

#if defined _MSC_VER
#define FUNC_NAME __FUNCDNAME__
#else
#define FUNC_NAME __func__
#endif
#define LOG_NONE 0x00
#define LOG_LINE 0x01

#define LOG(log_category, log_options, format, ...) {(void)log_category;(void)log_options; (void)printf(format, __VA_ARGS__);(void)printf("\r\n"); }

#ifdef DEBUG_LOG
#define LogDebug(FORMAT, ...) do { LOG(AZ_LOG_TRACE, LOG_LINE, FORMAT, __VA_ARGS__); } while((void)0,0)
#else
#define LogDebug(FORMAT, ...) do {} while((void)0,0)
#endif // DEBUG_LOG
#define LogError(FORMAT, ...) do { LOG(AZ_LOG_ERROR, LOG_LINE, FORMAT, __VA_ARGS__); } while((void)0,0)

#define INSERT_NODE_FAILURE     11

typedef struct NODE_INFO_TAG
{
    NODE_KEY key;
    void* data;
    struct NODE_INFO_TAG* right;
    struct NODE_INFO_TAG* left;
    // Keeps track of the child nodes balance
    // child on right = -1
    // child on left = +1
    int balance_factor;
} NODE_INFO;

typedef struct BINARY_TREE_INFO_TAG
{
    size_t items;
    size_t height;
    NODE_INFO* root_node;
} BINARY_TREE_INFO;

static NODE_INFO* create_new_node(NODE_KEY key_value, void* data)
{
    NODE_INFO* result;
    if ((result = (NODE_INFO*)malloc(sizeof(NODE_INFO))) == NULL)
    {
        LogError("Failure allocating tree node");
    }
    else
    {
        memset(result, 0, sizeof(NODE_INFO));
        result->key = key_value;
        result->data = data;
    }
    return result;
}

static void rotate_right(NODE_INFO* node_info)
{
    (void)node_info;
    NODE_INFO* parent_temp = node_info;
    node_info = parent_temp->left;



    /*NODE_INFO* parent_temp = node_info;
    node_info->left->right = node_info;*/


}

static void rotate_left(NODE_INFO* node_info)
{
    (void)node_info;
    /*node_info->right = temp->left;
    /*    temp->left = node_info;
    if (node_info == )*/


}

static void rebalance_if_neccessary(NODE_INFO* node_info)
{
    if (node_info->balance_factor < -1 && node_info->left->balance_factor == -1)
    {
        rotate_right(node_info);
        LogDebug("rotate right");
    }
    else if (node_info->balance_factor > 1 && node_info->right->balance_factor == 1)
    {
        rotate_left(node_info);
        LogDebug("rotate left");
    }
    else if (node_info->balance_factor < -1 && node_info->left->balance_factor == 1)
    {
        LogDebug("rotate left right");
    }
    else if (node_info->balance_factor > 1 && node_info->right->balance_factor == -1)
    {
        LogDebug("rotate right left");
    }
}

static int compare_node_values(const NODE_KEY* value_1, const NODE_KEY* value_2)
{
    if (*value_1 > *value_2) return 1;
    else if (*value_1 < *value_2) return -1;
    else return 0;
}

static const NODE_INFO* find_item(const NODE_INFO* node_info, const NODE_KEY* value)
{
    const NODE_INFO* result;
    if (node_info == NULL)
    {
        result = NULL;
    }
    else
    {
        int compare_value = compare_node_values(&node_info->key, value);
        if (compare_value > 0)
        {
            result = find_item(node_info->left, value);

        }
        else if (compare_value < 0)
        {
            result = find_item(node_info->right, value);
        }
        else
        {
            result = node_info;
        }
    }
    return result;
}

static int insert_into_tree(NODE_INFO** parent, NODE_INFO* new_node, size_t* height)
{
    int result = INSERT_NODE_FAILURE;
    if (*parent == NULL)
    {
        *parent = new_node;
        result = 0;
    }
    else if (new_node->key > (*parent)->key)
    {
        if (insert_into_tree(&(*parent)->right, new_node, height) != INSERT_NODE_FAILURE)
        {
            result = 1;
        }
        else
        {
            // Failure
            result = INSERT_NODE_FAILURE;
        }
    }
    else if (new_node->key < (*parent)->key)
    {
        if (insert_into_tree(&(*parent)->left, new_node, height) != INSERT_NODE_FAILURE)
        {
            result = -1;
        }
        else
        {
            // Failure 
            result = INSERT_NODE_FAILURE;
        }
    }

    if (result != INSERT_NODE_FAILURE)
    {
        (*height)++;
        (*parent)->balance_factor += result;
        rebalance_if_neccessary(*parent);
    }
    return result;
}

static void clear_tree(NODE_INFO* node_info)
{
    // Clear right
    if (node_info->right != NULL)
    {
        clear_tree(node_info->right);
        free(node_info->right);
    }
    // Clear left
    if (node_info->left != NULL)
    {
        clear_tree(node_info->left);
        free(node_info->left);
    }
}

BINARY_TREE_HANDLE binary_tree_create()
{
    BINARY_TREE_INFO* result = (BINARY_TREE_INFO*)malloc(sizeof(BINARY_TREE_INFO));
    if (result == NULL)
    {
        LogError("FAILURE: unable to allocate Binary tree info");
        result = NULL;
    }
    else
    {
        memset(result, 0, sizeof(BINARY_TREE_INFO));
    }
    return result;
}

void binary_tree_destroy(BINARY_TREE_HANDLE handle)
{
    if (handle != NULL)
    {
        if (handle->root_node != NULL)
        {
            clear_tree(handle->root_node);
            free(handle->root_node);
        }
        free(handle);
    }
}

int binary_tree_insert(BINARY_TREE_HANDLE handle, NODE_KEY value, void* data)
{
    int result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on insert");
        result = __LINE__;
    }
    else
    {
        size_t current_height = 0;
        NODE_INFO* new_node = create_new_node(value, data);
        if (new_node == NULL)
        {
            LogError("FAILURE: Creating new node on insert");
            result = __LINE__;
        }
        else if (insert_into_tree(&handle->root_node, new_node, &current_height) == INSERT_NODE_FAILURE)
        {
            LogError("FAILURE: Creating new node on insert");
            free(new_node);
            result = __LINE__;
        }
        else
        {
            handle->height = current_height;
            handle->items++;
            result = 0;
        }
    }
    return result;
}

int binary_tree_remove(BINARY_TREE_HANDLE handle, NODE_KEY value)
{
    (void)value;
    int result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on remove");
        result = __LINE__;
    }
    else
    {
        result = 0;
    }
    return result;
}

void* binary_tree_find(BINARY_TREE_HANDLE handle, NODE_KEY find_value)
{
    void* result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on find");
        result = NULL;
    }
    else
    {
        const NODE_INFO* node_info = find_item(handle->root_node, &find_value);
        if (node_info == NULL)
        {
            LogDebug("Item Not found");
            result = NULL;
        }
        else
        {
            result = node_info->data;
        }
    }
    return result;
}

size_t binary_tree_item_count(BINARY_TREE_HANDLE handle)
{
    size_t result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on remove");
        result = __LINE__;
    }
    else
    {
        result = handle->items;
    }
    return result;
}

size_t binary_tree_height(BINARY_TREE_HANDLE handle)
{
    size_t result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on remove");
        result = __LINE__;
    }
    else
    {
        result = handle->height;
    }
    return result;
}

void binary_tree_print(BINARY_TREE_HANDLE handle)
{
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on remove");
    }
    else
    {
    }
}