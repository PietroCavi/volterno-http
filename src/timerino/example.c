#include <stdio.h>
#include <unistd.h>

#include "timerino.h"

int to_measure(){
    int acc = 0;
    int compiler = 1;
    for(size_t i=0;i<100000000;i++){
        acc += 1;
        acc /= 2.3;
    }
    return acc;
}

int main(){
    printf("Timerino Section:\n");

    // starting the timer
    start_timerino();
  
    int acc = to_measure();

    // stopping the timer
    stop_timerino();
    
    // this is just to trick the compiler into not optimizing too much
    int compiler = 0; compiler += acc;

    // we can use get_timerino with a format to get the time in the unit that we want
    printf("Secondi:      %.2fs\n",  get_timerino("s")  );
    printf("Millisecondi: %.2fms\n", get_timerino("ms") );
    printf("Microsecondi: %.2fus\n", get_timerino("us") );
    printf("Nanosecondi:  %.2fns\n", get_timerino("ns") );
    
    printf("\n");
    
    printf("Timerone Section:\n");

    // creating a timerone
    timerone t = {0};
    
    // starting the timerone
    start_timerone(&t);
  
    acc = to_measure();

    // stopping the timer
    stop_timerone(&t);
    
    // this is just to trick the compiler into not optimizing too much
    compiler = 0; compiler += acc;

    // we can use get_timerino with a format to get the time in the unit that we want
    printf("Secondi:      %.2fs\n",  get_timerone(&t, "s")  );
    printf("Millisecondi: %.2fms\n", get_timerone(&t, "ms") );
    printf("Microsecondi: %.2fus\n", get_timerone(&t, "us") );
    printf("Nanosecondi:  %.2fns\n", get_timerone(&t, "ns") );
    
    printf("\n");
}

// to compile, stay in the directory of this file and the run: 
//      gcc example.c timerino.c -o example

