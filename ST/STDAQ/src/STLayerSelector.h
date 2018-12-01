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
#ifndef STLAYERSELECTOR_H
#define STLAYERSELECTOR_H 1

// Include files
// from STL
#include <string>
#include <vector>

// from GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// from Kernel
#include "Kernel/ISTChannelIDSelector.h"


/** @class STLayerSelector STLayerSelector.h
 *
 *  Algorithm to remove clusters in excluded layers
 *
 *  @author Ch. Elsasser
 *  @date   2012-05-02
 */

class STLayerSelector : public extends<GaudiTool, ISTChannelIDSelector>
{

public:
  using base_class::base_class;
  STLayerSelector ( const STLayerSelector & ) = delete;
  STLayerSelector& operator= ( const STLayerSelector& ) = delete;

  bool select (const LHCb::STChannelID& id ) const override;
  bool operator() ( const LHCb::STChannelID& id ) const override;

private:

  Gaudi::Property<std::string> m_detType { this,  "DetType", "TT" };
  Gaudi::Property<std::vector<std::string>> m_ignoredLayers { this, "IgnoredLayers" };

};

#endif // STLAYERSELECTOR_H
