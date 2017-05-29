
#include "Event/MCFun.h"

#include "Event/MCParticle.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SystemOfUnits.h"

double LHCb::MC::zInteraction(const LHCb::MCParticle* aParticle) 
{
  double z = 9999.*Gaudi::Units::m;
  for ( const auto & v : aParticle->endVertices() )
  {
    const auto zTest = v->position().z();
    if ( ( zTest < z ) && realInteraction( v->type() ) )  
    {
      z = zTest;
    } 
  }
  return z;
}

bool LHCb::MC::realInteraction(const LHCb::MCVertex::MCVertexType& type)
{
  return ! ( (type == LHCb::MCVertex::PhotoElectric)     ||
             (type == LHCb::MCVertex::RICHPhotoElectric) ||
             (type == LHCb::MCVertex::Cerenkov)          ||
             (type == LHCb::MCVertex::DeltaRay)          );
}
