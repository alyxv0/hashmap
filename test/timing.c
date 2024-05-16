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

char timer_print_buffer[20] = { 0 };

const char *timer_print(void) {
    double t = (double)(g_timer.end - g_timer.start);
    double x = t / 1000;
    int ctr = 0;

    while (t / 1000.0 > 1) {
        t = t / 1000.0;
        ctr++;
    }

    snprintf(timer_print_buffer, sizeof(timer_print_buffer), "%.2lf%s", t, time_postfix[ctr]);

    return (const char *) timer_print_buffer;
}

char byte_units_buffer[20] = { 0 };
char *byte_units_decimal[] = {
    "B",
    "kB",
    "MB",
    "GB",
    "TB",
    "PB",
    "EB",
    "ZB"
};

char *byte_units_binary[] = {
    "B",
    "KiB",
    "MiB",
    "GiB",
    "TiB",
    "PiB",
    "EiB",
    "ZiB"
};

const char *byte_units(size_t memlen, int prefix) {

    int i = 0;
    double len = (double) memlen;

    while((len / 1000.0) >= 1 && i != prefix) {
        len = len / 1000.0;
        i++;
    }

    snprintf(byte_units_buffer, sizeof(byte_units_buffer), "%.2lf %s", len, byte_units_binary[i]);

    return byte_units_buffer;
}