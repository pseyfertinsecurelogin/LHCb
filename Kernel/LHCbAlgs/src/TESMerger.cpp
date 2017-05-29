/** @class TESMerger TESMerger.h
 *
 *  Merge different track lists.
 *
 *  @author Sean Benson
 *  @date   24/02/2014
 */

#include <functional>
#include <string>
#include <vector>

#include "Event/Particle.h"
#include "Event/ProtoParticle.h"
#include "Event/Track.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObjectManager.h"
#include "GaudiKernel/KeyedTraits.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


template <class T> class TESMerger final : public GaudiAlgorithm
{

public:

  TESMerger(const std::string& name,
            ISvcLocator* pSvcLocator);

  StatusCode execute() override;

private:

  std::vector<std::string> m_inputLocations;
  std::string m_outputLocation;

};

template <class T>
TESMerger<T>::TESMerger(const std::string& name,
                        ISvcLocator* pSvcLocator):
  GaudiAlgorithm(name, pSvcLocator)
{
  // constructor
  declareProperty( "inputLocations",  m_inputLocations) ;
  declareProperty( "outputLocation", m_outputLocation) ;
}

template <class T>
StatusCode TESMerger<T>::execute()
{
  using ContT = KeyedContainer< T , Containers::HashMap >;

  auto * out = new ContT() ;
  put( out, m_outputLocation) ;

  for ( const auto & loc : m_inputLocations )
  {
    auto * cont_in = getIfExists<ContT>(loc) ;
    if ( cont_in )
    {
      for ( const auto * obj : *cont_in )
      { out->insert( obj->clone() ) ; }
    }
  }
  return StatusCode::SUCCESS;
}

typedef TESMerger<LHCb::ProtoParticle> TESMergerProtoParticle;
DECLARE_ALGORITHM_FACTORY( TESMergerProtoParticle )
typedef TESMerger<LHCb::Track> TESMergerTrack;
DECLARE_ALGORITHM_FACTORY( TESMergerTrack )
typedef TESMerger<LHCb::Particle> TESMergerParticle;
DECLARE_ALGORITHM_FACTORY( TESMergerParticle )
