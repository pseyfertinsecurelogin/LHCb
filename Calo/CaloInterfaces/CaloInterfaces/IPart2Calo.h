#ifndef IPART2CALO_H 
#define IPART2CALO_H 1

// Include files
// from STL
#include <string>

// from CaloInterfaces
#include "CaloInterfaces/ITrack2Calo.h"

namespace LHCb 
{
  class Particle;
  class ProtoParticle;
}



/** @class IPart2Calo IPart2Calo.h Kernel/IPart2Calo.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-06-25
 */
struct IPart2Calo : extend_interfaces<ITrack2Calo>
{
  // Return the interface ID
  DeclareInterfaceID( IPart2Calo, 2, 0 );

  using ITrack2Calo::match;
  virtual bool match(const  LHCb::Particle* part,
                     std::string det = DeCalorimeterLocation::Ecal,
                     CaloPlane::Plane plane = CaloPlane::ShowerMax, 
                     double delta = 0.
                     )= 0;
  virtual bool match(const  LHCb::ProtoParticle* proto,
                     std::string det = DeCalorimeterLocation::Ecal,
                     CaloPlane::Plane plane = CaloPlane::ShowerMax, 
                     double delta = 0.
                     )= 0;

  virtual bool inAcceptance()=0;  

};
#endif // IPART2CALO_H
