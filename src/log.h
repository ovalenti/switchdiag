#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 5
#endif

#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_VERB 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_TRACE 5

extern int log_dyn_level;

#define _LOG(lvl, ...)                                                         \
    do {                                                                       \
        if (log_dyn_level >= lvl)                                              \
            fprintf(stderr, __VA_ARGS__);                                      \
    } while (0)

#if (LOG_LEVEL >= LOG_LEVEL_ERR)
#define LOG_ERR(...) _LOG(LOG_LEVEL_ERR, "[1]" __VA_ARGS__)
#else
#define LOG_ERR(...)
#endif

#if (LOG_LEVEL >= LOG_LEVEL_WARN)
#define LOG_WARN(...) _LOG(LOG_LEVEL_WARN, "[2]" __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if (LOG_LEVEL >= LOG_LEVEL_VERB)
#define LOG_VERB(...) _LOG(LOG_LEVEL_VERB, "[3]" __VA_ARGS__)
#else
#define LOG_VERB(...)
#endif

#if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
#define LOG_DEBUG(...) _LOG(LOG_LEVEL_DEBUG, "[4]" __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if (LOG_LEVEL >= LOG_LEVEL_TRACE)
#define LOG_TRACE(...) _LOG(LOG_LEVEL_TRACE, "[5]" __VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

#define TRACE_ENTER LOG_TRACE("> %s\n", __func__)
#define TRACE_EXIT LOG_TRACE("< %s\n", __func__)

#endif /* LOG_H_ */
