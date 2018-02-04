// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stopwatch.h"
#include "logging.h"

typedef struct STOPWATCH_INFO_TAG* STOPWATCH_HANDLE;
typedef struct STOPWATCH_INFO_TAG
{
    clock_t start_time;
    clock_t stop_time;
    int started;
} STOPWATCH_INFO;

STOPWATCH_HANDLE stopwatch_create()
{
    STOPWATCH_INFO* result = (STOPWATCH_INFO*)malloc(sizeof(STOPWATCH_INFO));
    if (result == NULL)
    {
        LogError("FAILURE: unable to allocate stopwatch info");
        result = NULL;
    }
    else
    {
        memset(result, 0, sizeof(STOPWATCH_INFO));
    }
    return result;
}

void stopwatch_destroy(STOPWATCH_HANDLE handle)
{
    if (handle != NULL)
    {
        free(handle);
    }
}

int stopwatch_start(STOPWATCH_HANDLE handle)
{
    int result;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on start");
        result = __LINE__;
    }
    else
    {
        if (handle->started != 0)
        {
            LogError("FAILURE: Cannot start a inprogress timer");
            result = __LINE__;
        }
        else
        {
            handle->started = 1;
            handle->start_time = clock();
            handle->stop_time = clock();
        }
        result = 0;
    }
    return result;
}

void stopwatch_stop(STOPWATCH_HANDLE handle)
{
    if (handle != NULL)
    {
        handle->started = 0;
        handle->stop_time = clock();
    }
}

void stopwatch_reset(STOPWATCH_HANDLE handle)
{
    if (handle != NULL)
    {
        handle->start_time = clock();
        handle->stop_time = clock();
    }
}

double stopwatch_get_elapsed(STOPWATCH_HANDLE handle)
{
    double result;
    clock_t elapsed;
    if (handle == NULL)
    {
        LogError("FAILURE: Invalid handle specified on start");
        result = 0.0;
    }
    else
    {
        // If still in progress
        if (handle->started != 0)
        {
            elapsed = clock() - handle->start_time;
        }
        else
        {
            elapsed = handle->stop_time - handle->start_time;
        }
        result = ((double)elapsed) / CLOCKS_PER_SEC;
    }
    return result;
}

