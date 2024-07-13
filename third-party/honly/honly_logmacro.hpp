// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2024 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/honly
//


#ifndef LOGMACRO_HPP_HONLY
#define LOGMACRO_HPP_HONLY

#include <stdio.h>

// log tag
#ifndef HONLY_LOG_TAG
#define HONLY_LOG_TAG "HONLY"
#endif

// log level: DEBUG(3) > INFO(2) > WARN(1) > ERROR(0)
#ifndef HONLY_LOG_LEVEL
#define HONLY_LOG_LEVEL 3
#endif

#define HONLY_LOGD_LEVEL 3
#define HONLY_LOGI_LEVEL 2
#define HONLY_LOGW_LEVEL 1
#define HONLY_LOGE_LEVEL 0

#define HONLY_LOG_(fd, ...) \
do { \
    fprintf (fd, __VA_ARGS__); \
    fprintf (fd, "\033[0m\n"); \
} while(0)

#define HONLY_LOGP(...) \
do { \
    fprintf(stdout, "\033[30m[%s LOGP]: ", HONLY_LOG_TAG); \
    HONLY_LOG_(stdout, __VA_ARGS__); \
} while (0)

#define HONLY_LOGI(...) \
do { \
    if (HONLY_LOG_LEVEL >= HONLY_LOGI_LEVEL) { \
        fprintf(stdout, "\033[32m[%s LOGI]: %s: %s:%d - ", \
            HONLY_LOG_TAG, __func__, __FILE__, __LINE__); \
        HONLY_LOG_(stdout, __VA_ARGS__); \
    } \
} while (0)

#define HONLY_LOGD(...) \
do { \
    if (HONLY_LOG_LEVEL >= HONLY_LOGD_LEVEL) { \
        fprintf(stdout, "\033[37m[%s LOGD]: %s: %s:%d - ", \
            HONLY_LOG_TAG, __func__, __FILE__, __LINE__); \
        HONLY_LOG_(stdout, __VA_ARGS__); \
    } \
} while (0)

#define HONLY_LOGW(...) \
do { \
    if (HONLY_LOG_LEVEL >= HONLY_LOGW_LEVEL) { \
        fprintf(stdout, "\033[33m[%s LOGW]: %s: %s:%d - ", \
            HONLY_LOG_TAG, __func__, __FILE__, __LINE__); \
        HONLY_LOG_(stdout, __VA_ARGS__); \
    } \
} while (0)

#define HONLY_LOGE(...) \
do { \
    if (HONLY_LOG_LEVEL >= HONLY_LOGE_LEVEL) { \
        fprintf(stderr, "\033[31m[%s LOGE]: %s: %s:%d - ", \
            HONLY_LOG_TAG, __func__, __FILE__, __LINE__); \
        HONLY_LOG_(stdout, __VA_ARGS__); \
    } \
} while (0)

#endif