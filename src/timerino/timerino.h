#pragma once

#include <time.h>

// This first section is for "general flow", it's a singleton,
// it's easy to use, but not good to measure single thread performance
// in multithreaded applications.

void start_timerino();
void stop_timerino();
double get_timerino(char* format);

// By using a timerone we can generalized the singleton and use a
// struct to measure perfomance on different threads
// (I made this to be able to do timeouts on a multithreaded http server)

typedef struct{
    struct timespec start_timerone_struct, end_timerone_struct;
}timerone;

void start_timerone(timerone* stopwatch);
void stop_timerone(timerone* stopwatch);
double get_timerone(timerone* stopwatch, char* format);
