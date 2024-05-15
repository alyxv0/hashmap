#include "timing.h"
#include <stdio.h>
#include <math.h>

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

const char *time_postfix[] = {
    "mcs",
    "ms",
    "s",
};

char buffer[20] = { 0 };

const char *timer_print(void) {
    double t = (double)(g_timer.end - g_timer.start);
    double x = t / 1000;
    int ctr = 0;

    while (t / 1000.0 > 1) {
        t = t / 1000.0;
        ctr++;
    }

    snprintf(buffer, sizeof(buffer), "%.2lf%s", t, time_postfix[ctr]);

    return (const char *) buffer;
}
