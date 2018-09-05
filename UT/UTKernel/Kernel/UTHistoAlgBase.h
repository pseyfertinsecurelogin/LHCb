
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 *  @date   2018-09-04
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTHIUTOALGBASE_H
#define UTKERNEL_UTHIUTOALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiHistoAlg.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTHistoBase.h UTKernel/UTHistoBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
   */
  //-----------------------------------------------------------------------------

  class HistoAlgBase : public UT::CommonBase<GaudiHistoAlg>
  {

  public:

    /// Standard constructor
    HistoAlgBase( const std::string& name,
             ISvcLocator* pSvcLocator )
      : UT::CommonBase<GaudiHistoAlg> ( name, pSvcLocator ) { }

  };

}


#endif // UTKERNEL_UTALGBASE_H
