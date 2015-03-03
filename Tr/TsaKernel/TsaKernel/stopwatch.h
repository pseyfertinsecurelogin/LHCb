#ifndef _STOPWATCH_H
#define _STOPWATCH_H

// #include <time.h>

// A simple code timing class

#include "GaudiKernel/Timing.h"

namespace Tsa{
  class stopWatch{

  public:


    stopWatch():fSum(0.0){}

    ~stopWatch();  
    void   start();
    void stop();

    double sum() {return fSum;}
    double lasttime(){return fLasttime;}
    void reset();

  private:

    //  clock_t fStartTime;
    //  clock_t fStopTime;
    longlong m_startTime;
 
    double fSum;
    double fLasttime;
  };
};

#endif //_STOPWATCH_H

