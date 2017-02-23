#ifndef IReferenceRate_h
#define IReferenceRate_h

// Include files
#include "GaudiKernel/INamedInterface.h"

/** @class IReferenceRate IReferenceRate.h
 *
 *  functionality: provide a reference rate
 *
 *  @author Gerhard Raven and Wouter Hulsbergen
 *  @date   2009-01-01
 */


struct IReferenceRate : extend_interfaces<INamedInterface>
{
  DeclareInterfaceID( IReferenceRate, 2, 0 );

  virtual double rate() const = 0 ;
  virtual size_t tick() const = 0 ;
};
#endif // IReferenceRate_h
