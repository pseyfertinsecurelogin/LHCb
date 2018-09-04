
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  CVS Log :-
 *  $Id: UTTupleAlgBase.h,v 1.3 2009-03-09 15:14:26 akeune Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTTUPLEALGBASE_H
#define UTKERNEL_UTTUPLEALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTTupleBase.h UTKernel/UTTupleBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class TupleAlgBase : public UT::CommonBase<GaudiTupleAlg>
  {

  public:
    
    /// Standard constructor
    TupleAlgBase( const std::string& name,
                  ISvcLocator* pSvcLocator )
      : UT::CommonBase<GaudiTupleAlg> ( name, pSvcLocator ) { }
    
  };
  
}


#endif // UTKERNEL_UTALGBASE_H
