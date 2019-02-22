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
#ifndef VELOALGORITHMS_IMCVELOFETYPE_H
#define VELOALGORITHMS_IMCVELOFETYPE_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class IMCVeloFEType IMCVeloFEType.h VeloAlgorithms/IMCVeloFEType.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2005-11-16
 */

namespace LHCb {
  class MCVeloFE;
}

struct IMCVeloFEType : extend_interfaces<IAlgTool> {

public:
  DeclareInterfaceID( IMCVeloFEType, 2, 0 );

  enum Types { Signal = 0, Noise, Other }; // Return the interface ID
  virtual void FEType( const LHCb::MCVeloFE* fe, int& feType ) = 0;
};
#endif // VELOALGORITHMS_IMCVELOFETYPE_H
