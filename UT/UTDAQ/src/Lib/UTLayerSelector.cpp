/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
// local
#include "UTDAQ/UTLayerSelector.h"
// kernel
#include "Kernel/UTChannelID.h"
#include "Kernel/STLExtensions.h"

// ====================================================================
namespace {
  auto mp = [](const char* p, unsigned int i) { return std::make_pair(p,i); };
  static const auto s_layerMap = LHCb::make_array(
      // UT layer mapping
      mp("UTaX", 9), mp("UTaU",10), mp("UTbV",17), mp("UTbX",18));
}
// ====================================================================

DECLARE_COMPONENT( UTLayerSelector )

// ====================================================================

// ====================================================================
bool UTLayerSelector::select
(const LHCb::UTChannelID& id ) const
{
  return (*this)(id);
}
// ====================================================================

// ====================================================================
bool UTLayerSelector::operator() ( const LHCb::UTChannelID& id) const
{
  // Checks detector
  if ((m_detType == "UT" && id.isUT()))
  {
    if( msgLevel(MSG::DEBUG) ) debug() << "Excluded layers are in " << m_detType << ". Cluster is in other detector." <<endmsg;

    return false;
  }

  // Checks layer

  for (auto it = m_ignoredLayers.begin();it!=m_ignoredLayers.end();it++){
    if ( static_cast<int>(it->find(m_detType)) == -1){ // Checks if detector and layer agree
      continue;
    }
    auto jt = std::find_if(  s_layerMap.begin(), s_layerMap.end(),
                             [&]( const std::pair<const char*,unsigned int>& p ) {
                                 return id.uniqueLayer() == p.second && *it == p.first ;
                             } );
    if (jt!=s_layerMap.end()) {
        if( msgLevel(MSG::DEBUG) ) debug() << "Cluster is in "<< m_detType <<" layer "<< (*it) <<" and will be removed!" << endmsg;
        return true;
    }

    if( msgLevel(MSG::DEBUG) ) debug() << "Cluster will not be removed!"<<endmsg;
  }

  return false;
}
// ====================================================================
