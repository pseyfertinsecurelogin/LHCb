// $Id: SpacePoint.cpp,v 1.1.1.1 2006-07-24 14:56:20 mneedham Exp $
#include "TsaKernel/SpacePoint.h"


Tsa::SpacePoint::SpacePoint(const Gaudi::XYZPoint& aPoint, 
               double tX, double tY, 
	       Tsa::SpacePoint::pointType aType,
	       unsigned int station,
	       std::vector<Tsa::Cluster*>& clusCont):
 m_point(aPoint),
 m_tX(tX),
 m_tY(tY),
 m_type(aType),
 m_station(station),
 m_clusters(clusCont),
 m_pass(0)
{
  // constructer
}

Tsa::SpacePoint::SpacePoint(const Tsa::SpacePoint& point): KeyedObject<int>() {
 
  /// Copy constructer
  m_point = point.m_point;
  m_tX = point.m_tX;
  m_tY = point.m_tY;
  m_type = point.m_type; 
  m_station = point.m_station;
  m_pass = point.m_pass;

  // copy  clusters
  std::vector<Tsa::Cluster*>::const_iterator iterClus = point.m_clusters.begin();
  for(; iterClus != point.m_clusters.end(); ++iterClus){
    m_clusters.push_back(*iterClus);
  }
}

Tsa::SpacePoint* Tsa::SpacePoint::clone() const{
  return new Tsa::SpacePoint(*this);
}
