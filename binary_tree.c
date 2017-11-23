// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

#define USE_RECURSION

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
    struct NODE_INFO_TAG* parent;
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

static void print_tree(const NODE_INFO* node_info, size_t indent_level)
{
    if (node_info != NULL)
    {
        for (size_t index = 0; index < indent_level; index++)
            printf("\t");
        printf("%d\n", node_info->key);
        print_tree(node_info->left, indent_level + 1);
        print_tree(node_info->right, indent_level + 1);
    }
}

static void rotate_right(NODE_INFO* node_info)
{
    NODE_INFO* tpm_node = node_info;

    node_info->parent->left = node_info->left;
    node_info->parent->left->balance_factor = 0;
    node_info->parent->left->parent = node_info->parent;
    node_info->parent->balance_factor++;
    if (node_info->parent->left->right == NULL)
    {
        node_info->parent->left->right = tpm_node;
        tpm_node->parent = node_info->parent->left;
        tpm_node->left = tpm_node->right = NULL;
        tpm_node->balance_factor = 0;
    }
    else
    {
        tpm_node->left = node_info->parent->left->right;
        tpm_node->right = NULL;
        node_info->parent->left->right = tpm_node;
        tpm_node->balance_factor = -1;
    }
}

static void rotate_left(NODE_INFO* node_info)
{
    NODE_INFO* tpm_node = node_info;

    node_info->parent->right = node_info->right;
    node_info->parent->right->balance_factor = 0;
    node_info->parent->right->parent = node_info->parent;
    node_info->parent->balance_factor++;
    tpm_node->parent = node_info->parent->right;
    if (node_info->parent->right->left == NULL)
    {
        node_info->parent->right->left = tpm_node;
        tpm_node->right = tpm_node->left = NULL;
        tpm_node->balance_factor = 0;
    }
    else
    {
        tpm_node->right = node_info->parent->left->right;
        tpm_node->left = NULL;
        node_info->parent->right->left = tpm_node;
        tpm_node->balance_factor = 1;
    }
}

static int rebalance_if_neccessary(NODE_INFO* node_info)
{
    int result;
    if (node_info->balance_factor < -1 && node_info->left->balance_factor == -1)
    {
        rotate_right(node_info);
        result = 1;
        LogDebug("rotate right");
    }
    else if (node_info->balance_factor > 1 && node_info->right->balance_factor == 1)
    {
        rotate_left(node_info);
        result = -1;
        LogDebug("rotate left");
    }
    else if (node_info->balance_factor < -1 && node_info->left->balance_factor == 1)
    {
        // Do Left right rotation
        // Make the left child, a child of the left's right node
        node_info->left->right->left = node_info->left;
        node_info->left->parent = node_info->left->right;
        node_info->left->right->parent = node_info;
        node_info->left->right = NULL;
        node_info->left = node_info->left->right;
        // Now do a right rotation
        rotate_right(node_info);
        result = -1;
    }
    else if (node_info->balance_factor > 1 && node_info->right->balance_factor == -1)
    {
        // Do right left rotation
        node_info->right->left->right = node_info->right;
        node_info->right->parent = node_info->right->left;
        node_info->right->left->parent = node_info;
        node_info->right->left = NULL;
        node_info->right = node_info->right->left;
        // Do the left rotation
        rotate_left(node_info);
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

static int compare_node_values(const NODE_KEY* value_1, const NODE_KEY* value_2)
{
    if (*value_1 > *value_2) return 1;
    else if (*value_1 < *value_2) return -1;
    else return 0;
}

static NODE_INFO* find_node(NODE_INFO* node_info, const NODE_KEY* value)
{
    NODE_INFO* result;
    if (node_info == NULL)
    {
        result = NULL;
    }
    else
    {
#ifdef USE_RECURSION
        int compare_value = compare_node_values(&node_info->key, value);
        if (compare_value > 0)
        {
            result = find_node(node_info->left, value);

        }
        else if (compare_value < 0)
        {
            result = find_node(node_info->right, value);
        }
        else
        {
            result = node_info;
        }
#else
        int compare_value;
        const NODE_INFO* compare_node = node_info;

        result = NULL;
        while (compare_node != NULL)
        {
            compare_value = compare_node_values(&compare_node->key, value);
            if (compare_value > 0)
            {
                compare_node = compare_node->left;
            }
            else if (compare_value < 0)
            {
                compare_node = compare_node->right;
            }
            else
            {
                result = compare_node;
            }
        }
#endif
    }
    return result;
}

static int insert_into_tree(NODE_INFO** target_node, NODE_INFO* new_node, size_t* height)
{
    int result = INSERT_NODE_FAILURE;
#ifdef USE_RECURSION
    (*height)++;
    if (*target_node == NULL)
    {
        *target_node = new_node;
        result = 0;
    }
    else if (new_node->key > (*target_node)->key)
    {
        if (insert_into_tree(&(*target_node)->right, new_node, height) != INSERT_NODE_FAILURE)
        {
            if ((*target_node)->right->parent == NULL)
            {
                (*target_node)->right->parent = *target_node;
            }
            result = 1;
        }
        else
        {
            // Failure
            result = INSERT_NODE_FAILURE;
        }
    }
    else if (new_node->key < (*target_node)->key)
    {
        if (insert_into_tree(&(*target_node)->left, new_node, height) != INSERT_NODE_FAILURE)
        {
            if ((*target_node)->left->parent == NULL)
            {
                (*target_node)->left->parent = *target_node;
            }
            result = -1;
        }
        else
        {
            // Failure 
            result = INSERT_NODE_FAILURE;
        }
    }
#else
    int continue_run = 1;
    NODE_INFO* current_node = *target_node;
    NODE_INFO* prev_node = NULL;
    do
    {
        (*height)++;
        if (current_node == NULL)
        {
            current_node = new_node;
            current_node->parent = prev_node;
            result = continue_run = 0;
        }
        else if (new_node->key > current_node->key)
        {
            prev_node = current_node;
            current_node = current_node->right;
        }
        else if (new_node->key < current_node->key)
        {
            prev_node = current_node;
            current_node = current_node->left;
        }
        else
        {
            // Failure 
            continue_run = 0;
            result = INSERT_NODE_FAILURE;
        }
    } while (continue_run != 0);
#endif
    if (result != INSERT_NODE_FAILURE)
    {
        (*target_node)->balance_factor += result;
        if (rebalance_if_neccessary(*target_node) != 0)
        {
            // A rebalance will always subtract the height
            (*height)--;
        }
    }
    return result;
}

static int remove_node(NODE_INFO* node_info, const NODE_KEY* node_key)
{
    int result;
    NODE_INFO* current_node = find_node(node_info, node_key);
    if (current_node == NULL)
    {
        result = __LINE__;
    }
    else
    {
        NODE_INFO* previous_node = current_node->parent;

        // Remove the node
        result = 0;
        // Does the node have a single child
        if ((current_node->left != NULL && current_node->right == NULL) || (current_node->right != NULL && current_node->left == NULL))
        {
            // The right node is there
            if (current_node->left == NULL && current_node->right != NULL)
            {
                // If previous_node's left tree equals Node n
                if (previous_node->left == current_node)
                {
                    // then predecessor's left tree becomes n's right tree
                    // and delete n
                    previous_node->left = current_node->right;
                    free(current_node);
                    current_node = NULL;
                }
                // If predecessor's right tree equals Node n
                else
                {
                    // then predecessor's right tree becomes n's right tree
                    // and delete n
                    previous_node->right = current_node->right;
                    free(current_node);
                    current_node = NULL;
                }
            }
            else // Left node Present, No Right node Present
            {
                if (previous_node->left == current_node)
                {
                    previous_node->left = current_node->left;
                    free(current_node);
                    current_node = NULL;
                }
                else
                {
                    previous_node->right = current_node->left;
                    free(current_node);
                    current_node = NULL;
                }
            }
        }
        else if (current_node->left == NULL && current_node->right == NULL)
        {
            if (previous_node->left == current_node)
            {
                previous_node->left = NULL;
            }
            else
            {
                previous_node->right = NULL;
            }
            free(current_node);
        }
        // CASE 3: Node has two children
        // Replace Node with smallest value in right subtree
        else if (current_node->left != NULL && current_node->right != NULL)
        {
            NODE_INFO* check_node = current_node->right;
            if ( (current_node->left == NULL) && (current_node->right == NULL))
            {
                current_node = check_node;
                free(check_node);
                current_node->right = NULL;
            }
            else // Right child has children
            {
                // If the node's right child has a left child
                // Move all the way down left to locate smallest element
                if ((current_node->right)->left != NULL)
                {
                    NODE_INFO* left_current;
                    NODE_INFO* left_current_prev;
                    left_current_prev = current_node->right;
                    left_current = (current_node->right)->left;
                    while (left_current->left != NULL)
                    {
                        left_current_prev = left_current;
                        left_current = left_current->left;
                    }
                    current_node->data = left_current->data;
                    free(left_current);
                    left_current_prev->left = NULL;
                }
                else
                {
                    NODE_INFO* temp = current_node->right;
                    current_node->data = temp->data;
                    current_node->right = temp->right;
                    free(temp);
                }
            }
        }
    }
    return result;
}

static void clear_tree(NODE_INFO* node_info)
{
#ifdef USE_RECURSION
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
#else
    NODE_INFO* target_node = node_info;
    NODE_INFO* previous_node = NULL;

    while (target_node != NULL)
    {
        if (target_node->left == NULL)
        {
            target_node = target_node->right;
        }
        else
        {
            // Find the inorder predecessor of current
            previous_node = target_node->left;
            while (previous_node->right != NULL && previous_node->right != target_node)
            {
                previous_node = previous_node->right;
            }

            // Make current as right child of its inorder predecessor
            if (previous_node->right == NULL)
            {
                previous_node->right = target_node;
                target_node = target_node->left;
            }
            // Revert the changes made in if part to restore the original
            // tree i.e., fix the right child of predecssor
            else
            {
                previous_node->right = NULL;
                target_node = target_node->right;
            }
        }
    }
#endif
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
            LogError("FAILURE: Inserting new node");
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
        result = remove_node(handle->root_node, &value);
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
        const NODE_INFO* node_info = find_node(handle->root_node, &find_value);
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

void binary_tree_print_tree(BINARY_TREE_HANDLE handle)
{
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on print");
    }
    else
    {
        print_tree(handle->root_node, 0);
    }
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
