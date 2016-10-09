#ifndef ITRACK2CALO_H 
#define ITRACK2CALO_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "Event/State.h"
#include "Event/CaloPosition.h"
#include "CaloDet/DeCalorimeter.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/ParticleID.h"

// Forward declarations
namespace LHCb 
{
  class Track;
  class CaloHypo;
  class CaloCluster;
}



/** @class ITrack2Calo ITrack2Calo.h Kernel/ITrack2Calo.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-06-25
 */
struct ITrack2Calo : extend_interfaces<IAlgTool>
{
  // Return the interface ID
  DeclareInterfaceID(ITrack2Calo, 3, 0 );

  virtual bool  match(const  LHCb::Track* track,
                      std::string det = DeCalorimeterLocation::Ecal,
                      CaloPlane::Plane plane = CaloPlane::ShowerMax, 
                      double delta = 0.,
                      LHCb::ParticleID pid = LHCb::ParticleID(211)
                      )= 0;


  virtual LHCb::State caloState() = 0;
  virtual LHCb::CaloCellID caloCellID() = 0;
  virtual bool isValid() = 0;
  //
  virtual LHCb::State closestState(LHCb::CaloHypo*    hypo   ,LHCb::ParticleID pid = LHCb::ParticleID(211))=0;
  virtual LHCb::State closestState(LHCb::CaloCluster* cluster,LHCb::ParticleID pid = LHCb::ParticleID(211))=0;
  virtual LHCb::State closestState(LHCb::CaloPosition calopos,LHCb::ParticleID pid = LHCb::ParticleID(211))=0;
  virtual LHCb::State closestState(LHCb::CaloCellID   cellID ,LHCb::ParticleID pid = LHCb::ParticleID(211))=0;
  virtual const LHCb::Track* track()=0;  

};
#endif // ITRACK2CALO_H
