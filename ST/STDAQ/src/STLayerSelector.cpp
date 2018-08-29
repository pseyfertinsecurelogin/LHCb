// Include files
// local
#include "STLayerSelector.h"
// kernel
#include "Kernel/STChannelID.h"
#include "Kernel/STLExtensions.h"

// ====================================================================
namespace {
  auto mp = [](const char* p, unsigned int i) { return std::make_pair(p,i); };
  static const auto s_layerMap = std::array {
      // TT layer mapping
      mp("TTaX", 9), mp("TTaU",10), mp("TTbV",17), mp("TTbX",18),
      // IT layer mapping
      mp("T1X1", 9), mp("T1U", 10), mp("T1V", 11), mp("T1X2",12),
      mp("T2X1",17), mp("T2U", 18), mp("T2V", 19), mp("T2X2",20),
      mp("T3X1",25), mp("T3U", 26), mp("T3V", 27), mp("T3X2",28) };
}
// ====================================================================

DECLARE_COMPONENT( STLayerSelector )

// ====================================================================

// ====================================================================
bool STLayerSelector::select
(const LHCb::STChannelID& id ) const
{
  return (*this)(id);
}
// ====================================================================

// ====================================================================
bool STLayerSelector::operator() ( const LHCb::STChannelID& id) const
{
  // Checks detector
  if ((m_detType == "TT" && id.isIT()) || (m_detType == "IT" && id.isTT()))
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
