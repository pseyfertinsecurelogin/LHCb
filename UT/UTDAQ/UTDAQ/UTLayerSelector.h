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
#ifndef UTLAYERSELECTOR_H
#define UTLAYERSELECTOR_H 1

// Include files
// from STL
#include <string>
#include <vector>

// from GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// from Kernel
#include "Kernel/IUTChannelIDSelector.h"

/** @class UTLayerSelector UTLayerSelector.h
 *
 *  Algorithm to remove clusters in excluded layers
 *
 *  @author A. Beiter (based on code by Ch. Elsasser)
 *  @date   2018-09-04
 */

class UTLayerSelector : public extends<GaudiTool, IUTChannelIDSelector> {

public:
  using base_class::base_class;
  UTLayerSelector( const UTLayerSelector& ) = delete;
  UTLayerSelector& operator=( const UTLayerSelector& ) = delete;

  bool select( const LHCb::UTChannelID& id ) const override;
  bool operator()( const LHCb::UTChannelID& id ) const override;

private:
  Gaudi::Property<std::string>              m_detType{this, "DetType", "UT"};
  Gaudi::Property<std::vector<std::string>> m_ignoredLayers{this, "IgnoredLayers"};
};

#endif // UTLAYERSELECTOR_H
