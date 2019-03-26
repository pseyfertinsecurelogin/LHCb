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
#ifndef IFLAGSIGNALCHAIN_H
#define IFLAGSIGNALCHAIN_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCParticle;
}

/** @class IFlagSignalChain IFlagSignalChain.h
 *
 *
 *  @author Gloria Corti
 *  @date   2015-07-23
 */
struct IFlagSignalChain : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IFlagSignalChain, 2, 0 );

  /// Set flag if particle is signal or from signal
  virtual void setFromSignalFlag( const LHCb::MCParticle* mcPart ) = 0;
};
#endif // IFLAGSIGNALCHAIN_H
