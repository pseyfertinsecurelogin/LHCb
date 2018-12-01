/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
// Include files
// ============================================================================
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IToolSvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCSources.h"
#include "LoKi/MCAlgs.h"
#include "LoKi/MCExtract.h"
#include "LoKi/Services.h"
#include "LoKi/MCParticles1.h"
#include "LoKi/Trees.h"
#include "LoKi/Algs.h"
// ============================================================================
/** @file
 *  Implementation file for various sources
 *  @author Vanya BELYAEV ibelyav@physics.syr.edu
 *  @date 2006-12-07
 */
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc*      svc  ,
  const std::string&           path )
  : LoKi::AuxFunBase ( std::tie ( svc , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( true ) )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc*      svc  ,
  const LoKi::MCTypes::MCCuts& cuts ,
  const std::string&           path )
  : LoKi::AuxFunBase ( std::tie ( svc , cuts , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( cuts      )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc*      svc  ,
  const Decays::iNode&         node ,
  const std::string&           path )
  : LoKi::AuxFunBase ( std::tie ( svc , "<NODE>" , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( DecNode ( node ) )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc*         svc  ,
  const Decays::IMCDecay::Finder& finder ,
  const std::string&              path )
  : LoKi::AuxFunBase ( std::tie ( svc , "<FINDER>" , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true      )
  , m_finder     ( finder    )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc*         svc  ,
  const Decays::IMCDecay::iTree&  tree ,
  const std::string&              path )
  : LoKi::AuxFunBase ( std::tie ( svc , "<TREE>" , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true      )
  , m_finder     ( tree      )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const IDataProviderSvc* svc        ,
  const std::string&      descriptor ,
  const std::string&      path       )
  : LoKi::AuxFunBase ( std::tie ( svc , descriptor , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true       )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
  , m_decay      ( descriptor )
{
  //
  if ( gaudi() ) { buildFinder () ; }
}
// ============================================================================


// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm*        alg          ,
  const std::string&           path         , 
  const bool                   useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg , path , useRootInTES ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( true ) )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm*        alg          ,
  const LoKi::MCTypes::MCCuts& cuts         ,
  const std::string&           path         ,
  const bool                   useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg , cuts , path ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( cuts      )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm*        alg          ,
  const Decays::iNode&         node         ,
  const std::string&           path         ,
  const bool                   useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg, "<NODE>" , path , useRootInTES ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( DecNode ( node ) )
  , m_use_finder ( false     )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm*           alg          ,
  const Decays::IMCDecay::Finder& finder       ,
  const std::string&              path         ,
  const bool                      useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg, "<FINDER>" , path , useRootInTES ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true      )
  , m_finder     ( finder    )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm*           alg          ,
  const Decays::IMCDecay::iTree&  tree         ,
  const std::string&              path         , 
  const bool                      useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg, "<TREE>" , path , useRootInTES ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true      )
  , m_finder     ( tree      )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::SourceTES::SourceTES
( const GaudiAlgorithm* alg          ,
  const std::string&    descriptor   ,
  const std::string&    path         ,
  const bool            useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( alg , descriptor , path , useRootInTES ) )
  , SourceTES::_Base   ( alg , path , useRootInTES )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( false ) )
  , m_use_finder ( true       )
  , m_finder     ( Decays::Trees::Invalid_<const LHCb::MCParticle*>() )
  , m_decay      ( descriptor )
{
  //
  if ( gaudi() ) { buildFinder () ; }
}
// ============================================================================



// ============================================================================
void LoKi::MCParticles::SourceTES::buildFinder () const
{
  //
  if ( !use_finder() || m_finder.valid() ) { return ; }
  //
  LoKi::ILoKiSvc* ls = lokiSvc() ;
  SmartIF<IToolSvc> toolSvc ( ls ) ;
  Assert ( !(!toolSvc) , "Unable to locate tool service" ) ;
  //
  std::string factory = "LoKi::MCDecay" ;
  //
  Decays::IMCDecay* tool = nullptr ;
  StatusCode sc = toolSvc -> retrieveTool ( factory , tool ) ;
  Assert ( sc.isSuccess () , "Unable to retrieve '" + factory + "'" , sc ) ;
  Assert ( nullptr != tool       , "Decays::IMCDecay* points to NULL" ) ;
  //
  m_finder = tool -> tree ( m_decay) ;
  //
  toolSvc -> releaseTool ( tool ) ; // do not need the tool anymore
  //
  Assert ( !(!m_finder) , "the tree is invalid:  '" + m_decay + "'" ) ;
}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCParticles::SourceTES* 
LoKi::MCParticles::SourceTES::clone() const { return new SourceTES ( *this ) ; }
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
LHCb::MCParticle::ConstVector 
LoKi::MCParticles::SourceTES::operator() () const {
  //
  LHCb::MCParticle::ConstVector result ;
  //
  get ( path() , result ) ;
  //
  return result ;
}
// ============================================================================
// get the data from TES
// ============================================================================
std::size_t 
LoKi::MCParticles::SourceTES::get 
( const std::string&             location ,
  LHCb::MCParticle::ConstVector& output   ) const
{
  //
  const LHCb::MCParticle::Container* particles = 
    LoKi::TES::get_<LHCb::MCParticle::Container>( *this ) ;
  Assert ( particles , "No valid data is found at location '"+location+"'") ;
  //
  if ( m_use_finder )
  {
    if ( !m_finder && !m_decay.empty() ) { buildFinder() ; }
    if ( !m_finder )
    {
      const LoKi::Services& svcs = LoKi::Services::instance() ;
      StatusCode sc = m_finder.validate ( svcs.ppSvc() ) ;
      Assert ( sc.isSuccess () , "Unable to validate the DecayFinder" ) ;
    }
    //
    Assert   ( !(!m_finder)    , "Unable to validate the DecayFinder" ) ;
    // use finder!!
    m_finder.findDecay ( particles->begin () ,
                         particles->end   () ,
                         output          ) ;
  }
  else
  {
    // use cuts!
    std::copy_if( particles->begin ()  , 
                  particles->end   ()  ,
                  std::back_inserter ( output ) , std::cref( m_cut ) ) ;
  }
  //
  return output.size() ;
}
// ============================================================================
// get the data from TES
// ============================================================================
std::size_t 
LoKi::MCParticles::SourceTES::count
( const std::string& location  ) const
{
  //
  const LHCb::MCParticle::Container* particles = 
    LoKi::TES::get_<LHCb::MCParticle::Container>( *this ) ;
  Assert ( particles , "No valid data is found at location '"+location+"'") ;
  //
  if ( m_use_finder )
  {
    LHCb::MCParticle::ConstVector output ;
    if ( !m_finder && !m_decay.empty() ) { buildFinder() ; }
    if ( !m_finder )
    {
      const LoKi::Services& svcs = LoKi::Services::instance() ;
      StatusCode sc = m_finder.validate ( svcs.ppSvc() ) ;
      Assert ( sc.isSuccess () , "Unable to validate the DecayFinder" ) ;
    }
    //
    Assert   ( !(!m_finder)    , "Unable to validate the DecayFinder" ) ;
    // use finder!!
    m_finder.findDecay ( particles->begin () ,
                         particles->end   () ,
                         output          ) ;
    return output.size() ;
  }
  //
  // use cuts!
  //
  return LoKi::Algs::count_if ( particles->begin ()  ,
                                particles->end   ()  , std::cref ( m_cut ) ) ;
  //
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& 
LoKi::MCParticles::SourceTES::fillStream ( std::ostream& o ) const
{
  o << " MCSOURCE( '" << path() << "' , " ;
  if  ( m_use_finder ) { o << m_finder ; }
  else                 { o << m_cut    ; }
  //
  return o << " ) " ;
}
// ============================================================================



// ============================================================================
// constructor
// ============================================================================
LoKi::MCParticles::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const std::string&              path      )
  : TESData ( algorithm ,  
              path      ,  
              LoKi::BasicFunctors<const LHCb::MCParticle*>::BooleanConstant ( true ) ) 
{}
// ============================================================================
// constructor
// ============================================================================
LoKi::MCParticles::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const std::string&              path      ,
  const LoKi::MCTypes::MCCuts&    cuts      ) 
  : LoKi::AuxFunBase ( std::tie ( algorithm , path , cuts ) ) 
  , LoKi::BasicFunctors<const LHCb::MCParticle*>::Source() 
  , LoKi::TES::DataHandle<LHCb::MCParticle::Container>( algorithm , path )
  , m_cuts ( cuts )
{}
// ============================================================================
// constructor
// ============================================================================
LoKi::MCParticles::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const LoKi::MCTypes::MCCuts&    cuts      ,
  const std::string&              path      )
  : TESData ( algorithm , path , cuts ) 
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCParticles::TESData*
LoKi::MCParticles::TESData::clone() const 
{ return new LoKi::MCParticles::TESData(*this) ; }
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
LoKi::MCParticles::TESData::result_type
LoKi::MCParticles::TESData::operator() () const 
{
  const auto* data = get() ;
  result_type result ;
  result.reserve ( data -> size  () / 2 ) ;
  std::copy_if   ( data -> begin () , 
                   data -> end   () ,
                   std::back_inserter ( result ) , 
                   std::cref (  m_cuts ) ) ;
  return result ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::MCParticles::TESData::fillStream ( std::ostream& o ) const
{ return o << "MCTESDATA('" << location() << "'," << m_cuts << ")" ; }
// ============================================================================

// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCParticles::TESCounter::TESCounter
( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Source& s ) 
  : LoKi::AuxFunBase ( std::tie ( s ) )
  , LoKi::Functor<void,double> () 
  , m_source ( s ) 
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCParticles::TESCounter*
LoKi::MCParticles::TESCounter::clone() const 
{  return new LoKi::MCParticles::TESCounter(*this ) ; }
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
double LoKi::MCParticles::TESCounter::operator() (  ) const
{ return m_source().size() ; }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::MCParticles::TESCounter::fillStream ( std::ostream& o ) const
{ return o << "MCNUM(" << m_source << ")" ; }
// ============================================================================

// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCVertices::SourceTES::SourceTES
( const IDataProviderSvc*         svc  , 
  const std::string&              path ) 
  : LoKi::AuxFunBase ( std::tie ( svc , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut ( LoKi::BasicFunctors<const LHCb::MCVertex*>::BooleanConstant ( true ) )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCVertices::SourceTES::SourceTES
( const IDataProviderSvc*         svc  , 
  const LoKi::MCTypes::MCVCuts&   cuts ,
  const std::string&              path ) 
  : LoKi::AuxFunBase ( std::tie ( svc , cuts , path ) )
  , SourceTES::_Base   ( svc , path )
  , m_cut        ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCVertices::SourceTES::SourceTES
( const GaudiAlgorithm*           algorithm    ,
  const std::string&              path         ,
  const bool                      useRootInTES ) 
  : LoKi::AuxFunBase ( std::tie ( algorithm , path , useRootInTES ) )
  , SourceTES::_Base   ( algorithm , path , useRootInTES )
  , m_cut        ( LoKi::BasicFunctors<const LHCb::MCVertex*>::BooleanConstant ( true ) )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::MCVertices::SourceTES::SourceTES
( const GaudiAlgorithm*           algorithm    , 
  const LoKi::MCTypes::MCVCuts&   cuts         ,
  const std::string&              path         ,
  const bool                      useRootInTES ) 
  : LoKi::AuxFunBase ( std::tie ( algorithm , cuts , path , useRootInTES ) )
  , SourceTES::_Base   ( algorithm , path , useRootInTES )
  , m_cut        ( cuts )
{}
// ============================================================================
//  MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCVertices::SourceTES* 
LoKi::MCVertices::SourceTES::clone() const { return new SourceTES ( *this ) ; }
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
LHCb::MCVertex::ConstVector 
LoKi::MCVertices::SourceTES::operator() () const
{
  
  const LHCb::MCVertex::Container* vertices = 
    LoKi::TES::get_<LHCb::MCVertex::Container>( *this ) ;
  Assert ( vertices , "No valid data is found at location '" + location() + "'") ;
  //
  LHCb::MCVertex::ConstVector result ;
  // use cuts!
  std::copy_if( vertices->begin ()  ,
                vertices->end   ()  ,
                std::back_inserter ( result ) , std::cref(m_cut) ) ;
  //
  return result ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& 
LoKi::MCVertices::SourceTES::fillStream ( std::ostream& o ) const
{ return o << " MCVSOURCE( '" << path () << "', " << m_cut << " ) " ; }

// ============================================================================
// constructor
// ============================================================================
LoKi::MCVertices::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const std::string&              path      )
  : TESData ( algorithm ,  
              path      ,  
              LoKi::BasicFunctors<const LHCb::MCVertex*>::BooleanConstant ( true ) ) 
{}
// ============================================================================
// constructor
// ============================================================================
LoKi::MCVertices::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const std::string&              path      ,
  const LoKi::MCTypes::MCVCuts&   cuts      ) 
  : LoKi::AuxFunBase ( std::tie ( algorithm , path , cuts ) ) 
  , LoKi::BasicFunctors<const LHCb::MCVertex*>::Source() 
  , LoKi::TES::DataHandle<LHCb::MCVertex::Container>( algorithm , path )
  , m_cuts ( cuts )
{}
// ============================================================================
// constructor
// ============================================================================
LoKi::MCVertices::TESData::TESData 
( const GaudiAlgorithm*           algorithm , 
  const LoKi::MCTypes::MCVCuts&   cuts      ,
  const std::string&              path      )
  : TESData ( algorithm , path , cuts ) 
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCVertices::TESData*
LoKi::MCVertices::TESData::clone() const 
{ return new LoKi::MCVertices::TESData(*this) ; }
// ============================================================================
// MANDATORY: the only essential method:
// ============================================================================
LoKi::MCVertices::TESData::result_type
LoKi::MCVertices::TESData::operator() () const 
{
  const auto* data = get() ;
  result_type result ;
  result.reserve ( data -> size  () / 2 ) ;
  std::copy_if   ( data -> begin () , 
                   data -> end   () ,
                   std::back_inserter ( result ) , 
                   std::cref (  m_cuts ) ) ;
  return result ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::MCVertices::TESData::fillStream ( std::ostream& o ) const
{ return o << "MCVTESDATA('" << location() << "'," << m_cuts << ")" ; }
// ============================================================================


// ============================================================================
// The END
// ============================================================================
