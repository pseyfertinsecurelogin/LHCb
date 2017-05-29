#include "STDet/DeUTLayer.h"

#include <stddef.h>
#include <algorithm>
#include <numeric>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "Kernel/UTNames.h"
#include "STDet/DeUTModule.h"
#include "STDet/DeUTSector.h"

class DeSTSector;

using namespace LHCb;

/** @file DeUTLayer.cpp
 *
 *  Implementation of class :  DeUTLayer
 *
 *  @author Jianchun Wang, Matt Needham
 *  @date   2012-07-10
 *
 */

DeUTLayer::DeUTLayer( const std::string& name ) :
  DeSTLayer( name ),
  m_parent(NULL)
{
  m_modules.clear();
}


const CLID& DeUTLayer::clID () const
{
  return DeUTLayer::classID() ;
}


StatusCode DeUTLayer::initialize()
{
  // initialize method
  MsgStream msg(msgSvc(), name() );
  StatusCode sc = DeSTLayer::initialize();
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    m_parent = getParent<DeUTLayer>();
    STChannelID parentID = m_parent->elementID();
    STChannelID chan(STChannelID::typeUT, parentID.station(), id(), 0, 0, 0);
    setElementID(chan);
    m_modules = getChildren<DeUTLayer>();
    flatten();
    m_nickname = UTNames().UniqueLayerToString(chan);
  }
  return sc;
}


DeUTModule* DeUTLayer::findModule(const STChannelID aChannel)
{
  auto iter = std::find_if(m_modules.begin() , m_modules.end(),
                           [&](const DeUTModule* m) { return m->contains(aChannel);});
  return iter != m_modules.end() ? *iter: nullptr;
}


DeUTModule* DeUTLayer::findModule(const Gaudi::XYZPoint& point)
{
  auto iter = std::find_if(m_modules.begin(), m_modules.end(),
                           [&](const DeUTModule* m) { return m->isInside(point);});
  return iter != m_modules.end() ? *iter: nullptr;
}


void DeUTLayer::flatten()
{
  DeUTLayer::Children::const_iterator iterModule = modules().begin();
  for (;iterModule != modules().end(); ++iterModule){
    DeUTModule* tModule = *iterModule;
    DeUTModule::Children::const_iterator iterSector = tModule->sectors().begin();
    for ( ; iterSector !=  tModule->sectors().end() ; ++iterSector ){
      DeSTSector* tSector = *iterSector;
      m_sectors.push_back(tSector);
    }
  }
}


double DeUTLayer::fractionActive() const
{
  return std::accumulate(m_modules.begin(), m_modules.end(), 0.0,
                [](double f, const DeUTModule* m) { return f + m->fractionActive(); }
         )/double(m_modules.size());
}
