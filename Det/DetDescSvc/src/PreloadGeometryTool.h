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
#ifndef PRELOADGEOMETRYTOOL_H
#define PRELOADGEOMETRYTOOL_H 1

// STL
#include <memory>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/IGenericTool.h" // Interface

/** @class PreloadGeometryTool PreloadGeometryTool.h
 *  Tool to traverse the full geometry tree and load it
 *
 *  @author Marco Cattaneo
 *  @date   2006-07-26
 */
struct PreloadGeometryTool : extends<GaudiTool, IGenericTool> {
  /// Standard constructor
  PreloadGeometryTool( const std::string& type, const std::string& name, const IInterface* parent );

  void execute() override; ///< Load the geometry tree
};

#endif // PRELOADGEOMETRYTOOL_H
