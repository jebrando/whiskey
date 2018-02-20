// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else // __cplusplus
#include <stdio.h>
#include <stddef.h>
#endif // __cplusplus

typedef struct BINARY_TREE_INFO_TAG* BINARY_TREE_HANDLE;

typedef void (*tree_remove_callback)(void* data);

// Used as the type value
typedef unsigned long NODE_KEY;

extern BINARY_TREE_HANDLE binary_tree_create();
extern void binary_tree_destroy(BINARY_TREE_HANDLE handle, tree_remove_callback delete_callback);

extern int binary_tree_insert(BINARY_TREE_HANDLE handle, NODE_KEY value, void* data);
extern int binary_tree_remove(BINARY_TREE_HANDLE handle, NODE_KEY value, tree_remove_callback remove_callback);
extern void* binary_tree_find(BINARY_TREE_HANDLE handle, NODE_KEY find_value);


// Diagnostic function
extern size_t binary_tree_item_count(BINARY_TREE_HANDLE handle);
extern size_t binary_tree_height(BINARY_TREE_HANDLE handle);
extern char* binary_tree_construct_visual(BINARY_TREE_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif  /* BINARY_TREE_H */
