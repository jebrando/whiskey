// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else // __cplusplus
#include <stdio.h>
#include <stddef.h>
#endif // __cplusplus

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

#ifdef __cplusplus
}
#endif

#endif  /* LOGGING_H */
