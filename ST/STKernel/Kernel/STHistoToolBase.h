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

#ifndef STKERNEL_STHISTOTOOLBASE_H
#define STKERNEL_STHISTOTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"

// local
#include "Kernel/STCommonBase.h"

namespace ST {

  //-----------------------------------------------------------------------------
  /** @class AlgBase STHistoToolBase.h STKernel/STHistoToolBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class HistoToolBase : public ST::CommonBase<GaudiHistoTool> {

  public:
    /// Standard constructor
    HistoToolBase( const std::string& type, const std::string& name, const IInterface* parent )
        : ST::CommonBase<GaudiHistoTool>( type, name, parent ) {}
  };

} // namespace ST

#endif // STKERNEL_STHISTOTOOLBASE_H
