// $Id: selection.h,v 1.11 2007-06-29 14:11:55 ibelyaev Exp $
// ====================================================================
// CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.11 $ 
// ====================================================================
// Incldue files
// ====================================================================
// STD & STL
// ====================================================================
#include <string>
#include <vector>
// ====================================================================
// Relations
// ====================================================================
#include "Relations/RelationsDict.h"
// ====================================================================
// Provided include files: 
// ====================================================================
#include "Event/Particle.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "Event/ProtoParticle.h"
#include "Event/Track.h"
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"
#include "Event/GenCollision.h"
#include "Event/RecVertex.h"
#include "HepMC/GenParticle.h"
// ====================================================================
// Other include files: put your own includes here:
// ====================================================================
//#include ...
//#include ...
//#include ...
// ====================================================================

/** @file 
  *       
  * Helper file for building Reflex dictionaries for Relations 
   * @author ibelyaev 
  * @date  2007-06-29 16:04:56.445930 
  *       
  */      

namespace Dict
{
  struct __Instantiations 
  {
GaudiDict::Relation1DDict<LHCb::MCVertex,LHCb::GenCollision>	_1 ;
GaudiDict::Relation1DDict<LHCb::Track,float>	_2 ;
GaudiDict::Relation2DDict<HepMC::GenParticle*,LHCb::MCParticle>	_3 ;
GaudiDict::Relation2DDict<HepMC::GenParticle*,LHCb::Particle>	_4 ;
GaudiDict::Relation2DDict<LHCb::MCParticle,HepMC::GenParticle*>	_5 ;
GaudiDict::Relation2DDict<LHCb::MCParticle,LHCb::Particle>	_6 ;
GaudiDict::Relation2DDict<LHCb::Particle,HepMC::GenParticle*>	_7 ;
GaudiDict::Relation2DDict<LHCb::Particle,LHCb::MCParticle>	_8 ;
GaudiDict::RelationWeighted1DDict<LHCb::CaloCluster,LHCb::MCParticle,float>	_9 ;
GaudiDict::RelationWeighted1DDict<LHCb::CaloDigit,LHCb::MCParticle,float>	_10 ;
GaudiDict::RelationWeighted1DDict<LHCb::MCParticle,LHCb::CaloCluster,float>	_11 ;
GaudiDict::RelationWeighted1DDict<LHCb::Particle,LHCb::VertexBase,double>	_12 ;
GaudiDict::RelationWeighted1DDict<LHCb::ProtoParticle,LHCb::MCParticle,double>	_13 ;
GaudiDict::RelationWeighted2DDict<LHCb::CaloCluster,LHCb::Track,float>	_14 ;
GaudiDict::RelationWeighted2DDict<LHCb::CaloHypo,LHCb::Track,float>	_15 ;
GaudiDict::RelationWeighted2DDict<LHCb::GenCollision,LHCb::RecVertex,std::pair<unsigned int,float> >	_16 ;
GaudiDict::RelationWeighted2DDict<LHCb::MCParticle,LHCb::Particle,double>	_17 ;
GaudiDict::RelationWeighted2DDict<LHCb::MCParticle,LHCb::Track,double>	_18 ;
GaudiDict::RelationWeighted2DDict<LHCb::MCVertex,LHCb::RecVertex,std::pair<unsigned int,float> >	_19 ;
GaudiDict::RelationWeighted2DDict<LHCb::Particle,LHCb::MCParticle,double>	_20 ;
GaudiDict::RelationWeighted2DDict<LHCb::RecVertex,LHCb::GenCollision,std::pair<unsigned int,float> >	_21 ;
GaudiDict::RelationWeighted2DDict<LHCb::RecVertex,LHCb::MCVertex,std::pair<unsigned int,float> >	_22 ;
GaudiDict::RelationWeighted2DDict<LHCb::Track,LHCb::CaloCluster,float>	_23 ;
GaudiDict::RelationWeighted2DDict<LHCb::Track,LHCb::CaloHypo,float>	_24 ;
GaudiDict::RelationWeighted2DDict<LHCb::Track,LHCb::MCParticle,double>	_25 ;
  };
};

// ====================================================================
// The END ============================================================
// ====================================================================
