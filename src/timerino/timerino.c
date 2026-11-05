#include "timerino.h"
#include <string.h>

struct timespec start_timerino_struct, end_timerino_struct;

inline void start_timerino(){
    clock_gettime(CLOCK_MONOTONIC, &start_timerino_struct);
}

inline void stop_timerino(){
    clock_gettime(CLOCK_MONOTONIC, &end_timerino_struct);
}

inline double get_timerino(char* format){
    long seconds = end_timerino_struct.tv_sec - start_timerino_struct.tv_sec;
    long nanoseconds = end_timerino_struct.tv_nsec - start_timerino_struct.tv_nsec;

    double total_ns = (seconds * 1000000000.0) + nanoseconds;
    
    if(strcmp(format, "s") == 0){
        return total_ns * 0.000000001;
    }
    else if(strcmp(format, "ms") == 0){
        return total_ns * 0.000001;
    }
    else if(strcmp(format, "us") == 0){
        return total_ns * 0.001;
    }
    else if(strcmp(format, "ns") == 0){
        return total_ns;
    }

    return 0.0;
}


inline void start_timerone(timerone* stopwatch){
    clock_gettime(CLOCK_MONOTONIC, &(stopwatch->start_timerone_struct));
}

inline void stop_timerone(timerone* stopwatch){
    clock_gettime(CLOCK_MONOTONIC, &(stopwatch->end_timerone_struct));
}

inline double get_timerone(timerone* stopwatch, char* format){
    long seconds = stopwatch->end_timerone_struct.tv_sec - stopwatch->start_timerone_struct.tv_sec;
    long nanoseconds = stopwatch->end_timerone_struct.tv_nsec - stopwatch->start_timerone_struct.tv_nsec;

    double total_ns = (seconds * 1000000000.0) + nanoseconds;
    
    if(strcmp(format, "s") == 0){
        return total_ns * 0.000000001;
    }
    else if(strcmp(format, "ms") == 0){
        return total_ns * 0.000001;
    }
    else if(strcmp(format, "us") == 0){
        return total_ns * 0.001;
    }
    else if(strcmp(format, "ns") == 0){
        return total_ns;
    }

    return 0.0;
}
