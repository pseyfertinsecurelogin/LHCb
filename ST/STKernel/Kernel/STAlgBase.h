
//-----------------------------------------------------------------------------
/** @file STAlgBase.h
 *
 *  Header file for algorithm base class : ST::AlgBase
 *
 *  CVS Log :-
 *  $Id: STAlgBase.h,v 1.1 2008-10-14 08:47:05 mneedham Exp $
 *
 *  @author Matthew Needham   
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef STKERNEL_STALGBASE_H
#define STKERNEL_STALGBASE_H 1

#include <functional>
#include <string>

#include "/tmp/foobarpath/LHCb/ST/STKernel/src/STCommonBase.icpp"
// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "Kernel/STCommonBase.h"

class ISvcLocator;

namespace ST
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase STAlgBase.h STKernel/STAlgBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class AlgBase : public ST::CommonBase<GaudiAlgorithm>
  {

  public:

    /// Standard constructor
    AlgBase( const std::string& name,
             ISvcLocator* pSvcLocator )
      : ST::CommonBase<GaudiAlgorithm> ( name, pSvcLocator ) { }

  };

}


#endif // STKERNEL_STALGBASE_H
