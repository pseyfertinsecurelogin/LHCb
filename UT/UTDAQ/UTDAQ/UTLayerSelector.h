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
 *  @author Ch. Elsasser
 *  @date   2012-05-02
 */

class UTLayerSelector : public extends<GaudiTool, IUTChannelIDSelector>
{

public:
  using base_class::base_class;
  UTLayerSelector ( const UTLayerSelector & ) = delete;
  UTLayerSelector& operator= ( const UTLayerSelector& ) = delete;

  bool select (const LHCb::UTChannelID& id ) const override;
  bool operator() ( const LHCb::UTChannelID& id ) const override;

private:

  Gaudi::Property<std::string> m_detType { this,  "DetType", "UT" };
  Gaudi::Property<std::vector<std::string>> m_ignoredLayers { this, "IgnoredLayers" };

};

#endif // UTLAYERSELECTOR_H
