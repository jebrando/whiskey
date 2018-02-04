// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef STOPWATCH_H
#define STOPWATCH_H

#ifdef __cplusplus
#include <cstdio>
#include <ctime>
extern "C" {
#else // __cplusplus
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#endif // __cplusplus

typedef struct STOPWATCH_INFO_TAG* STOPWATCH_HANDLE;

extern STOPWATCH_HANDLE stopwatch_create();
extern void stopwatch_destroy(STOPWATCH_HANDLE handle);

extern int stopwatch_start(STOPWATCH_HANDLE handle);
extern void stopwatch_stop(STOPWATCH_HANDLE handle);
extern void stopwatch_reset(STOPWATCH_HANDLE handle);

// Amount of time elasped in seconds
extern double stopwatch_get_elapsed(STOPWATCH_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif  /* STOPWATCH_H */
