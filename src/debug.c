#include "internal/sq_commom.h"
#include "internal/sq_debug.h"
#include <stdarg.h>

void sampquery_log(const unsigned char debug_opt, const char *fmt, ...)
{
#ifdef _SAMPQUERY_DEBUG_
    if (debug_opt == 1)
    {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        return;
    }
#endif
    if (!debug_opt)
    {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        return;
    }
}