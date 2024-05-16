#ifndef TIMING_H
#define TIMING_H

#include <time.h>

void timer_start(void);
void timer_stop(void);
double timer_time(void);
const char *timer_print(void);
const char *byte_units(size_t memlen, int prefix);

#define BYTE     0
#define KILOBYTE 1
#define MEGABYTE 2
#define GIGABYTE 3
#define TERABYTE 4
#define PETABYTE 5
#define EXABYTE  6
#define ZETTABYTE 7


#endif