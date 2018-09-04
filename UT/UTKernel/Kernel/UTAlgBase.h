
//-----------------------------------------------------------------------------
/** @file UTAlgBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  CVS Log :-
 *  $Id: UTAlgBase.h,v 1.1 2008-10-14 08:47:05 mneedham Exp $
 *
 *  @author Matthew Needham   
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTALGBASE_H
#define UTKERNEL_UTALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTAlgBase.h UTKernel/UTAlgBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class AlgBase : public UT::CommonBase<GaudiAlgorithm>
  {

  public:

    /// Standard constructor
    AlgBase( const std::string& name,
             ISvcLocator* pSvcLocator )
      : UT::CommonBase<GaudiAlgorithm> ( name, pSvcLocator ) { }

  };

}


#endif // UTKERNEL_UTALGBASE_H
