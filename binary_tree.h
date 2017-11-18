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

// Used as the type value
typedef char NODE_VALUE;

extern BINARY_TREE_HANDLE binary_tree_create();
extern void binary_tree_destroy(BINARY_TREE_HANDLE handle);

extern int binary_tree_insert(BINARY_TREE_HANDLE handle, NODE_VALUE value);
extern int binary_tree_remove(BINARY_TREE_HANDLE handle, NODE_VALUE value);
extern int binary_tree_find(BINARY_TREE_HANDLE handle, NODE_VALUE find_value);


// Diagnostic function
extern size_t binary_tree_item_count(BINARY_TREE_HANDLE handle);
extern size_t binary_tree_height(BINARY_TREE_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif  /* BINARY_TREE_H */
