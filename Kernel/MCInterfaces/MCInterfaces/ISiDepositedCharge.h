#ifndef _ISiDepositedCharge_H
#define _ISiDepositedCharge_H

#include "GaudiKernel/IAlgTool.h"

namespace LHCb{
 class MCHit;
}

/** @class ISiDepositedCharge ISiDepositedCharge.h Kernel/ISiDepositedCharge.h
 *
 *  Interface Class for estimating deposited charge in Silicon
 *
 *  @author M.Needham
 *  @date   13/3/2002
 */

// Declaration of the interface ID ( interface id, major version, minor version) 

struct ISiDepositedCharge : extend_interfaces<IAlgTool> {

  /** Static access to interface id */
  DeclareInterfaceID(ISiDepositedCharge, 1 , 0); 
   
  /** calculate deposited charge (in electrons)
  * @param  aHit hit 
  * @return deposited charge 
  */
  virtual double charge(const LHCb::MCHit* aHit) const=0;

};

#endif // _ISiDepositedCharge_H
