
//-----------------------------------------------------------------------------
/** @file UTAlgBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 *  @date   2018-09-04
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
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date  2018-09-04 
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
