#include "checkm8.h"
#include "tool/payload.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "dev/types.h"

#ifdef CHECKM8_LOGGING

#include <stdarg.h>
#include <execinfo.h>

#endif

void checkm8_debug_indent(const char *format, ...)
{
#ifdef CHECKM8_LOGGING
    void *traces[100];
    int depth = backtrace(traces, 100) - 5;
    for(int i = 0; i < depth; i++)
    {
        printf("\t");
    }
    va_list args;

    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

void checkm8_debug_block(const char *format, ...)
{
#ifdef CHECKM8_LOGGING
    va_list args;

    va_start (args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

int main()
{
    struct pwned_device *dev = exploit_device();

    if(dev == NULL || dev->status == DEV_NORMAL)
    {
        printf("Failed to exploit device\n");
        return -1;
    }

    fix_heap(dev);
    demote_device(dev);

    // install some payloads or data and run them

    uninstall_all_data(dev);
    uninstall_all_payloads(dev);

    // crash!
    execute_gadget(dev, 0, 0, 0);
    free_device(dev);
    return 0;
}











