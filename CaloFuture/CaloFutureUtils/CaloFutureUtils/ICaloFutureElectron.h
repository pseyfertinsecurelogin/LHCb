#ifndef ICALOFUTUREELECTRON_H 
#define ICALOFUTUREELECTRON_H 1

// Include files
// from STL
#include <string>

#include "CaloFutureInterfaces/IPart2CaloFuture.h"
// from LHCb
#include "CaloFutureUtils/CaloMomentum.h"

// Forward declarations
namespace LHCb 
{
  class Particle;
  class ProtoParticle;
}


/** @class ICaloFutureElectron ICaloFutureElectron.h Kernel/ICaloFutureElectron.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2006-11-30
 */
struct ICaloFutureElectron : public extend_interfaces<IPart2CaloFuture> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureElectron, 4, 0 );

  virtual bool  set(const  LHCb::Particle* particle, 
                    std::string det = DeCalorimeterLocation::Ecal,
                    CaloPlane::Plane plane = CaloPlane::ShowerMax,
                    double delta =0 ) = 0;
  virtual bool  set(const  LHCb::ProtoParticle* proto, 
                    std::string det = DeCalorimeterLocation::Ecal,
                    CaloPlane::Plane plane = CaloPlane::ShowerMax,
                    double delta =0 ) = 0;
  virtual double             eOverP()= 0 ;
  virtual double             ecalE() = 0 ;
  virtual LHCb::CaloHypo*    electron() = 0 ;
  virtual LHCb::CaloHypo*    bremstrahlung() = 0 ;
  virtual LHCb::CaloMomentum bremCaloFutureMomentum() = 0 ;

  using ITrack2CaloFuture::closestState;
  virtual LHCb::State        closestState(std::string toWhat = "hypo") =0;

  virtual double caloTrajectoryZ(CaloPlane::Plane refPlane = CaloPlane::ShowerMax,std::string toWhat = "hypo") =0;
  virtual double caloTrajectoryL(CaloPlane::Plane refPlane = CaloPlane::ShowerMax ,std::string toWhat = "hypo") =0 ;

};
#endif // ICALOFUTUREELECTRON_H
