// $Id: STCluster.cpp,v 1.4 2006-10-11 15:36:09 mneedham Exp $
#include "TsaKernel/STCluster.h"

Tsa::STCluster* Tsa::STCluster::clone() const {
  return new Tsa::STCluster(*this);
}

LHCb::LHCbID Tsa::STCluster::id() const{
  return LHCb::LHCbID(channelID());
}

bool Tsa::STCluster::sameID(const LHCb::LHCbID testID) const{
  return(id() == testID);
}

const CLID& Tsa::STCluster::clID() const 
{
  return Tsa::STCluster::classID();
}

const CLID& Tsa::STCluster::classID()
{
  return Tsa::CLID_STCluster;
}

unsigned int Tsa::STCluster::weight() const{
  return 2;
}
