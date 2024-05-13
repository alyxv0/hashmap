#include "timing.h"

struct timer
{
    clock_t start;
    clock_t end;
};

__thread struct timer g_timer = {0};

void timer_start(void)
{
    g_timer.start = clock();
}

void timer_stop(void)
{
    g_timer.end = clock();
}

double timer_time(void)
{
    return (double)(g_timer.end - g_timer.start);
}
