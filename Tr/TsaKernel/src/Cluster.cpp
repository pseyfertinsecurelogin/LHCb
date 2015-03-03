//Id: Cluster.cpp,v 1.4 2006/09/08 16:01:43 mneedham Exp $

#include "TsaKernel/Cluster.h"
#include "Kernel/Trajectory.h"



Tsa::Cluster::~Cluster(){
  // destructer
}

Tsa::Cluster::Cluster(const Tsa::Cluster& other):
  ContainedObject(),
  m_error(other.m_error),
  m_isHot(other.m_isHot),
  m_pass(other.m_pass),
  m_onTrack(other.m_onTrack),
  m_yMin(other.m_yMin),
  m_yMax(other.m_yMax)
{
  // copy constructer
  m_trajectory.reset( (other.m_trajectory)->clone().release());
}
