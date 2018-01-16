
#pragma once

#include <time.h>

unsigned long long int time_diff( struct timespec *start, 
                                  struct timespec *stop ) 
{
  const auto diff_nsec = stop->tv_nsec - start->tv_nsec;
  const auto diff_sec  = stop->tv_sec  - start->tv_sec;
  return ( diff_nsec < 0 ?
           ( 1000000000ULL * (diff_sec - 1) ) + diff_nsec + 1000000000ULL :
           ( 1000000000ULL *  diff_sec      ) + diff_nsec                 );
}
