#ifndef TIMING_H
#define TIMING_H

#include <time.h>

void timer_start(void);

void timer_stop(void);

double timer_time(void);

const char *time_print(void);

#endif