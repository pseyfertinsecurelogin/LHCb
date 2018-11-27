/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

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
