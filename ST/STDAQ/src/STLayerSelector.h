#ifndef STLAYERSELECTOR_H
#define STLAYERSELECTOR_H 1

// Include files
// from STL
#include <string>
#include <vector>
#include <map>

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


class STLayerSelector :
  public virtual ISTChannelIDSelector,
  public GaudiTool
{
  friend struct ToolFactory<STLayerSelector>;

public:

  bool select (const LHCb::STChannelID& id ) const override;
  bool operator() ( const LHCb::STChannelID& id ) const override;
  StatusCode initialize() override;    ///< Algorithm initialization

protected:

  STLayerSelector ( const std::string& type,
                    const std::string& name,
                    const IInterface* parent);

private:

  STLayerSelector ();
  STLayerSelector ( const STLayerSelector & );
  STLayerSelector& operator= ( const STLayerSelector& );

  std::string m_detType;
  std::vector<std::string> m_ignoredLayers;

  std::map<std::string, unsigned int> m_layerMap;


};





#endif // STLAYERSELECTOR_H
