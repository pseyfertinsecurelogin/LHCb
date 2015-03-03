// $Id: OTCluster.cpp,v 1.6 2007-06-18 15:20:05 mneedham Exp $
#include "TsaKernel/OTCluster.h"
#include "OTDet/DeOTDetector.h"
#include "OTDet/DeOTModule.h"

#include <memory>



/// Copy constructor
Tsa::OTCluster::OTCluster(const Tsa::OTCluster& other):
Tsa::Cluster(other),
m_driftRadius(other.m_driftRadius),
m_det(other.m_det),
m_cluster(other.m_cluster){

}

Tsa::OTCluster* Tsa::OTCluster::clone() const {
  return new Tsa::OTCluster(*this);
}

Tsa::OTCluster::~OTCluster(){
  // destructer
}

const CLID& Tsa::OTCluster::clID() const 
{
  return Tsa::OTCluster::classID();
}

const CLID& Tsa::OTCluster::classID()
{
  return Tsa::CLID_OTCluster;
}


double Tsa::OTCluster::driftRadius(const double x,
                                   const double y) const
{

  const double dx = x - endPoint().x();
  const double dy = y - endPoint().y();
  const double timeAlongWire = m_det->propagationDelay()*sqrt(dx*dx + dy*dy); 
  return (m_det->driftDistance(m_cluster->calibratedTime()-timeAlongWire));
}

LHCb::LHCbID Tsa::OTCluster::id() const{
  return LHCb::LHCbID(channelID());
}


bool Tsa::OTCluster::sameID(const LHCb::LHCbID testID) const{
  return(id() == testID);
}

unsigned int Tsa::OTCluster::weight() const{
  return 1;
}
