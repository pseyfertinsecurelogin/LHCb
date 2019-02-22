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
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  @author Andy Beiter
 *  @date   2018-09-04
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTTOOLBASE_H
#define UTKERNEL_UTTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT {

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTToolBase.h UTKernel/UTToolBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
   */
  //-----------------------------------------------------------------------------

  class ToolBase : public UT::CommonBase<GaudiTool> {

  public:
    /// Standard constructor
    ToolBase( const std::string& type, const std::string& name, const IInterface* parent )
        : UT::CommonBase<GaudiTool>( type, name, parent ) {}
  };

} // namespace UT

#endif // UTKERNEL_UTALGBASE_H
