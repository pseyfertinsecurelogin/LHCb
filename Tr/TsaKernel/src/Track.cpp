#include "TsaKernel/Track.h"

Tsa::Track::Track(const double x,
                   const double y,
                   const double z,
                   const double tx,
                   const double ty, 
                   const double qDivP,
                   const std::vector<Tsa::Cluster*>& clusters):  
  m_z(z),
  m_clusters(clusters)
{

 m_stateVector = Gaudi::TrackVector();
 m_stateVector(0)  = x;
 m_stateVector(1)  = y; 
 m_stateVector(2)  = tx;
 m_stateVector(3)  = ty;
 m_stateVector(4)  = qDivP;

 m_covVector = Gaudi::TrackVector();

}

Tsa::Track::Track(const Tsa::Track& track): KeyedObject<int>() {
 
  /// Copy constructer
  m_z = track.m_z;
  m_chi2 = track.m_chi2;
  m_stateVector = track.m_stateVector;
  m_covVector = track.m_covVector;

  // copy  clusters
  std::vector<Tsa::Cluster*>::const_iterator iterClus = track.m_clusters.begin();
  for(; iterClus != track.m_clusters.end(); ++iterClus){
    m_clusters.push_back(*iterClus);
  }
}

Tsa::Track* Tsa::Track::clone() const{
  return new Tsa::Track(*this);
}
