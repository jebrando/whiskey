// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "binary_tree.h"
#include "logging.h"

#define TREE_COLOR      uint8_t
#define BLACK_COLOR     0
#define RED_COLOR       1
#define NUM_OF_CHARS    8

//static const char* KEY_VALUE_FMT = "%s%x";
static const char* KEY_VALUE_FMT = "%s%u";
static const char LEFT_PARENTHESIS = '(';
static const char RIGHT_PARENTHESIS = ')';
static const char* RED_NODE_FLAG = "*";
static const char* BLACK_NODE_FLAG = "";

typedef struct NODE_INFO_TAG
{
    NODE_KEY key;
    TREE_COLOR node_color;
    struct NODE_INFO_TAG* parent;
    struct NODE_INFO_TAG* right;
    struct NODE_INFO_TAG* left;
    void* data;
} NODE_INFO;

typedef struct BINARY_TREE_INFO_TAG
{
    size_t items;
    size_t height;
    NODE_INFO* root_node;
} BINARY_TREE_INFO;

static bool is_right_node(const NODE_INFO* node_info)
{
    bool result = false;
    if (node_info->parent != NULL)
    {
        result = (node_info->parent->right == node_info);
    }
    return result;
}

static bool is_left_node(const NODE_INFO* node_info)
{
    bool result = false;
    if (node_info->parent != NULL)
    {
        result = (node_info->parent->left == node_info);
    }
    return result;
}

static bool is_red(const NODE_INFO* node_info)
{
    bool result = false;
    if (node_info != NULL)
    {
        result = (node_info->node_color == RED_COLOR);
    }
    return result;
}

static bool is_black(const NODE_INFO* node_info)
{
    bool result = false;
    if (node_info != NULL)
    {
        result = (node_info->node_color == BLACK_COLOR);
    }
    return result;
}

static NODE_INFO* get_grandparent(const NODE_INFO* node_info)
{
    NODE_INFO* result = NULL;
    if (node_info->parent != NULL)
    {
        result = node_info->parent->parent;
    }
    return result;
}

static NODE_INFO* get_sibling(const NODE_INFO* node_info)
{
    NODE_INFO* result = NULL;
    if (node_info->parent != NULL)
    {
        if (node_info->parent->left == node_info)
            result = node_info->parent->right;
        else
            result = node_info->parent->left;
    }
    return result;
}

static NODE_INFO* get_uncle(const NODE_INFO* node_info)
{
    NODE_INFO* result = NULL;
    if (node_info->parent != NULL)
    {
        result = get_sibling(node_info->parent);
    }
    return result;
}

static void construct_visual_representation(const NODE_INFO* node_info, char* visualization)
{
    /*
            10
           /  \
          5    15
         / \   / \
        3   7 11  18
        .
        10(5(3)(7))(15(11)(18)))
    */
    // [1, 2, 3, 4]
    // 1(2(*4))(3)
    if (node_info != NULL)
    {
        size_t pos = 0;
        const NODE_INFO* prev = NULL;
        const NODE_INFO* current = node_info;
        char temp[8];
        bool upward_movement = false;

        while (current != NULL)
        {
            if (!upward_movement)
            {
                int len = sprintf(temp, KEY_VALUE_FMT, (current->node_color == RED_COLOR) ? RED_NODE_FLAG : BLACK_NODE_FLAG, current->key);
                memcpy(visualization + pos, temp, len);
                pos += len;
            }

            upward_movement = false;
            if (current->left != NULL && prev != current->left && prev != current->right)
            {
                memcpy(visualization + pos, &LEFT_PARENTHESIS, 1);
                pos += 1;
                prev = current;
                current = current->left;
            }
            else if (current->right != NULL && prev != current->right)
            {
                memcpy(visualization + pos, &LEFT_PARENTHESIS, 1);
                pos += 1;
                prev = current;
                current = current->right;
            }
            else
            {
                upward_movement = true;
                if (current->parent == NULL)
                {
                    // we're at the root and we came from the right node
                    current = NULL;
                }
                else
                {
                    memcpy(visualization + pos, &RIGHT_PARENTHESIS, 1);
                    pos += 1;

                    prev = current;
                    current = current->parent;
                }
            }
        }
    }
}

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
        result->node_color = RED_COLOR;
        result->key = key_value;
        result->data = data;
    }
    return result;
}

static void rotate_right(NODE_INFO** root_node, NODE_INFO* pivot_node)
{
    NODE_INFO* orig_parent = pivot_node->parent;
    if (*root_node == orig_parent)
    {
        *root_node = pivot_node;
    }
    else
    {
        if (is_right_node(orig_parent))
        {
            orig_parent->parent->right = pivot_node;
        }
        else
        {
            orig_parent->parent->left = pivot_node;
        }
    }
    pivot_node->parent = orig_parent->parent;

    // Move the left node to the parent
    if (pivot_node->right != NULL)
    {
        if (orig_parent->left == pivot_node)
        {
            orig_parent->left = pivot_node->right;
        }
        else
        {
            orig_parent->right = pivot_node->right;
        }
        pivot_node->right->parent = orig_parent;
    }
    else
    {
        orig_parent->left = NULL;
    }
    pivot_node->right = orig_parent;
    orig_parent->parent = pivot_node;
    
    pivot_node->node_color = BLACK_COLOR;
    pivot_node->right->node_color = RED_COLOR;
}

static void rotate_left(NODE_INFO** root_node, NODE_INFO* pivot_node)
{
    NODE_INFO* orig_parent = pivot_node->parent;
    if (*root_node == orig_parent)
    {
        *root_node = pivot_node;
    }
    else
    {
        if (is_right_node(orig_parent))
        {
            orig_parent->parent->right = pivot_node;
        }
        else
        {
            orig_parent->parent->left = pivot_node;
        }
    }
    pivot_node->parent = orig_parent->parent;

    // Move the left node to the parent
    if (pivot_node->left != NULL)
    {
        if (orig_parent->right == pivot_node)
        {
            orig_parent->right = pivot_node->left;
        }
        else
        {
            orig_parent->left = pivot_node->left;
        }
        pivot_node->left->parent = orig_parent;
    }
    else
    {
        orig_parent->right = NULL;
    }

    pivot_node->left = orig_parent;
    orig_parent->parent = pivot_node;

    pivot_node->node_color = BLACK_COLOR;
    pivot_node->left->node_color = RED_COLOR;
}

static void double_rotate_left_right(NODE_INFO** root_node, NODE_INFO* pivot_node)
{
    pivot_node->right->parent = pivot_node->parent;
    pivot_node->parent->left = pivot_node->right;
    pivot_node->parent = pivot_node->right;
    if (pivot_node->parent->left != NULL)
    {
        pivot_node->right = pivot_node->parent->left;
        pivot_node->right->parent = pivot_node;
    }
    else
    {
        pivot_node->right = NULL;
    }
    pivot_node->parent->left = pivot_node;

    rotate_right(root_node, pivot_node->parent);
}

static void double_rotate_right_left(NODE_INFO** root_node, NODE_INFO* pivot_node)
{
    pivot_node->left->parent = pivot_node->parent;
    pivot_node->parent->right = pivot_node->left;
    pivot_node->parent = pivot_node->left;
    if (pivot_node->parent->right != NULL)
    {
        pivot_node->left = pivot_node->parent->right;
        pivot_node->left->parent = pivot_node;
    }
    else
    {
        pivot_node->left = NULL;
    }
    pivot_node->parent->right = pivot_node;

    rotate_left(root_node, pivot_node->parent);
}

static NODE_INFO* rebalance_if_neccessary(NODE_INFO** root_node, NODE_INFO* node_info, size_t* height)
{
    NODE_INFO* result;
    if (node_info == *root_node)
    {
        node_info->node_color = BLACK_COLOR;
        result = NULL;
    }
    // is the parent black
    else if (is_black(node_info->parent))
    {
        // Everything is find nothing to do here
        result = NULL;
    }
    else // Parent is red
    {
        result = NULL;
        bool right_node = is_right_node(node_info);
        NODE_INFO* uncle = get_uncle(node_info);
        NODE_INFO* grandparent = get_grandparent(node_info);
        if (uncle == NULL)
        {
            // Need to do a rotation
            if (right_node && grandparent->left == node_info->parent)
            {
                double_rotate_left_right(root_node, node_info->parent);
                (*height)--;
            }
            else if (!right_node && grandparent->right == node_info->parent)
            {
                double_rotate_right_left(root_node, node_info->parent);
                (*height)--;
            }
            else if (right_node && grandparent->right == node_info->parent)
            {
                rotate_left(root_node, node_info->parent);
                (*height)--;
            }
            else if (!right_node && grandparent->left == node_info->parent)
            {
                rotate_right(root_node, node_info->parent);
                (*height)--;
            }
        }
        else if (is_red(uncle) )
        {
            // Check to make sure that the grandparent isn't the root
            if (grandparent->parent != NULL)
            {
                // The grandparent is not the root node
                // push down blackness from grandparent ...
                node_info->parent->node_color = BLACK_COLOR;
                uncle->node_color = BLACK_COLOR;
                grandparent->node_color = RED_COLOR;

                // ... so recheck the node with the grandparent
                result = grandparent;
            }
            else
            {
                // Since the parent's parent is the root then 
                // we just need to recolor
                node_info->parent->node_color = BLACK_COLOR;
                uncle->node_color = BLACK_COLOR;
            }
        }
        else
        {
            result = NULL;
            // The uncle is black so we do the rotation
            if (right_node && grandparent->left == node_info->parent)
            {
                // Double swap
                double_rotate_left_right(root_node, node_info->parent);
                (*height)--;
            }
            else if (right_node)
            {
                rotate_left(root_node, node_info->parent);
            }
            else if (!right_node && grandparent->right == node_info->parent)
            {
                // Double swap
                double_rotate_right_left(root_node, node_info->parent);
                (*height)--;
            }
            else
            {
                rotate_right(root_node, node_info->parent);
                (*height)--;
            }
        }
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
        int compare_value;
        NODE_INFO* compare_node = node_info;

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
                break;
            }
        }
    }
    return result;
}

static int insert_into_tree(NODE_INFO** root_node, NODE_INFO* new_node, size_t* height)
{
    int result;
    int continue_run = 1;
    size_t incremental_height = 1;
    if (*root_node == NULL)
    {
        // Insert the root
        *root_node = new_node;
        result = 0;
    }
    else
    {
        NODE_INFO* prev_node = *root_node;
        NODE_INFO* current_node = *root_node;
        do
        {
            if (new_node->key > current_node->key)
            {
                if (current_node->right == NULL)
                {
                    current_node->right = new_node;
                    current_node->right->parent = current_node;
                    continue_run = 0;
                    if (current_node->left == NULL)
                    {
                        // If no left node then we need to increment the height
                        incremental_height++;
                    }
                }
                else
                {
                    prev_node = current_node;
                    current_node = current_node->right;
                    incremental_height++;
                }
                result = 0;
            }
            else if (new_node->key < current_node->key)
            {
                if (current_node->left == NULL)
                {
                    current_node->left = new_node;
                    current_node->left->parent = current_node;
                    continue_run = 0;
                    if (current_node->right == NULL)
                    {
                        // If no left node then we need to increment the height
                        incremental_height++;
                    }
                }
                else
                {
                    prev_node = current_node;
                    current_node = current_node->left;
                    incremental_height++;
                }
                result = 0;
            }
            else
            {
                // Failure the keys are equal
                continue_run = 0;
                result = __LINE__;
            }
        } while (continue_run != 0);
    }
    if (result == 0)
    {
        NODE_INFO* rebalance_node = new_node;
        do
        {
            rebalance_node = rebalance_if_neccessary(root_node, rebalance_node, &incremental_height);
        } while (rebalance_node != NULL);

        if (incremental_height > *height)
        {
            *height = incremental_height;
        }
    }
    return result;
}

static int remove_node(NODE_INFO** root_node, const NODE_KEY* node_key, tree_remove_callback remove_callback)
{
    int result;
    NODE_INFO* node_info = *root_node;
    NODE_INFO* del_node = find_node(node_info, node_key);
    if (del_node == NULL)
    {
        result = __LINE__;
    }
    else
    {
        //NODE_INFO* previous_node = current_node->parent;
        if (remove_callback != NULL)
        {
            remove_callback(del_node->data);
        }

        // Remove the node
        result = 0;
        // 1. If node has no children delete it
        if (del_node->left == NULL && del_node->right == NULL)
        {
            bool left_node = is_left_node(del_node);

            // Remove the node
            if (left_node)
            {
                del_node->parent->left = NULL;
            }
            else
            {
                del_node->parent->right = NULL;
            }

            if (del_node->parent == *root_node)
            {
                NODE_INFO* sibling_node = get_sibling(del_node);
                if (sibling_node != NULL && sibling_node->node_color == BLACK_COLOR)
                {
                    // double black has black sibling, l
                    if (left_node)
                    {
                        if (sibling_node->right != NULL && sibling_node->right->node_color == RED_COLOR)
                        {
                            rotate_left(root_node, sibling_node);
                        }
                        else
                        {
                            // double black node had black sibling, but the nephew is black
                            double_rotate_right_left(root_node, sibling_node);
                        }
                    }
                    else
                    {
                        if (sibling_node->left != NULL && sibling_node->left->node_color == RED_COLOR)
                        {
                            rotate_right(root_node, sibling_node);
                        }
                        else
                        {
                            double_rotate_left_right(root_node, sibling_node);
                        }
                    }
                }
            }
        }
        // 2. Node has one child - delete it and replace it with that child
        else if ((del_node->left == NULL && del_node->right != NULL) || (del_node->right == NULL && del_node->left != NULL))
        {
            if (del_node->left == NULL && del_node->right != NULL)
            {
                // right child
                if (is_left_node(del_node))
                {
                    del_node->parent->left = del_node->right;
                }
                else
                {
                    del_node->parent->right = del_node->right;
                }
                del_node->right->parent = del_node->parent;
                del_node->right->node_color = BLACK_COLOR;
            }
            else
            {
                // right child
                if (is_left_node(del_node))
                {
                    del_node->parent->left = del_node->left;
                }
                else
                {
                    del_node->parent->right = del_node->left;
                }
                del_node->left->parent = del_node->parent;
                del_node->left->node_color = BLACK_COLOR;
            }
        }
        // 3. If Node has two children, replace the node with the min value on right side
        else
        {
            bool delete_parent_left = true;
            NODE_INFO* large_node = del_node->left;
            // Find the largest node in the left subtree
            while (large_node->right == NULL)
            {
                delete_parent_left = false;
                large_node = large_node->right;
            }
            
            if (del_node == *root_node)
            {
                *root_node = large_node;
            }

            if (delete_parent_left)
            {
                large_node->parent->left = large_node->left;
            }
            else
            {
                large_node->parent->right = large_node->left;
            }
            large_node->parent = del_node->parent;
            large_node->right = del_node->right;
            if (large_node != del_node->left)
            {
                large_node->left = del_node->left;
            }
            else
            {
                large_node->left = NULL;
            }
            //large_node->node_color = del_node->node_color;
        }
        free(del_node);
     }
    return result;
}

static void clear_tree(NODE_INFO* node_info, tree_remove_callback delete_callback)
{
    NODE_INFO* current = node_info;

    while (current != NULL)
    {
        if (current->left != NULL)
        {
            current = current->left;
        }
        else if (current->right != NULL)
        {
            current = current->right;
        }
        else
        {
            // Has no children
            NODE_INFO* target = current;
            if (current->parent != NULL && current->parent->left == current)
            {
                current->parent->left = NULL;
            }
            else if (current->parent != NULL)
            {
                current->parent->right = NULL;
            }
            current = current->parent;
            if (delete_callback != NULL)
            {
                delete_callback(target->data);
            }
            free(target);
        }
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

void binary_tree_destroy(BINARY_TREE_HANDLE handle, tree_remove_callback delete_callback)
{
    if (handle != NULL)
    {
        if (handle->root_node != NULL)
        {
            clear_tree(handle->root_node, delete_callback);
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
        size_t current_height = handle->height;
        NODE_INFO* new_node = create_new_node(value, data);
        if (new_node == NULL)
        {
            LogError("FAILURE: Creating new node on insert");
            result = __LINE__;
        }
        else if (insert_into_tree(&handle->root_node, new_node, &current_height) != 0)
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

int binary_tree_remove(BINARY_TREE_HANDLE handle, NODE_KEY value, tree_remove_callback remove_callback)
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
        result = remove_node(&handle->root_node, &value, remove_callback);
        if (result == 0)
        {
            handle->items-- ;
        }
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

char* binary_tree_construct_visual(BINARY_TREE_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on construct visual");
        result = NULL;
    }
    else
    {
        // Allocate the result
        if (handle->items > 0)
        {
            size_t len = (handle->items*NUM_OF_CHARS+1) + (handle->items * 2);
            result = (char*)malloc(len + 1);
            memset(result, 0, len + 1);
            construct_visual_representation(handle->root_node, result);
        }
        else
        {
            result = (char*)malloc(1);
            result[0] = '\0';
        }
    }
    return result;
}