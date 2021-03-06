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

#ifndef STKERNEL_STTUPLETOOLBASE_H
#define STKERNEL_STTUPLETOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleTool.h"

// local
#include "Kernel/STCommonBase.h"

namespace ST {

  //-----------------------------------------------------------------------------
  /** @class AlgBase STTupleToolBase.h STKernel/STTupleToolBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class TupleToolBase : public ST::CommonBase<GaudiTupleTool> {

  public:
    /// Standard constructor
    TupleToolBase( const std::string& type, const std::string& name, const IInterface* parent )
        : ST::CommonBase<GaudiTupleTool>( type, name, parent ) {}
  };

} // namespace ST

#endif // STKERNEL_STHISTOTOOLBASE_H
