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

//-----------------------------------------------------------------------------
/** @file STHistoBase.h
 *
 *  Header file for algorithm base class : ST::AlgBase
 *
 *  CVS Log :-
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef STKERNEL_STTUPLEALGBASE_H
#define STKERNEL_STTUPLEALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"

// local
#include "Kernel/STCommonBase.h"

namespace ST {

  //-----------------------------------------------------------------------------
  /** @class AlgBase STTupleBase.h STKernel/STTupleBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class TupleAlgBase : public ST::CommonBase<GaudiTupleAlg> {

  public:
    /// Standard constructor
    TupleAlgBase( const std::string& name, ISvcLocator* pSvcLocator )
        : ST::CommonBase<GaudiTupleAlg>( name, pSvcLocator ) {}
  };

} // namespace ST

#endif // STKERNEL_STALGBASE_H
