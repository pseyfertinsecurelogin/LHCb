// defintion of the stopwatch class
// simple timer - with start and stop
// m. needham 17/10/99

#include "TsaKernel/stopwatch.h"


Tsa::stopWatch::~stopWatch(){

  //stopwatch destructer
}


void Tsa::stopWatch::start(){

  m_startTime = System::currentTime();
}

void Tsa::stopWatch::stop(){

 fLasttime = double(System::currentTime() - m_startTime);
 fSum += fLasttime;
}

void Tsa::stopWatch::reset(){
  fSum = 0.0;
}

