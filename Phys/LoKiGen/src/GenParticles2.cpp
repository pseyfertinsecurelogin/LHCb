/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Kinematics.h"
#include "LoKi/GenParticles2.h"
#include "LoKi/GenChild.h"
#include "LoKi/Trees.h"
#include "LoKi/IGenDecay.h"
// ============================================================================
/** @file
 *  Implementation file for functirons from file  LoKi/GenParticles2.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vaya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-20
 *
 */
namespace LoKi { namespace GenParticles {
// ============================================================================
// constructor from child-selectors
// ============================================================================
PolarizationAngle::PolarizationAngle ( const LoKi::GenChild::Selector& daughter     ,
                                       const LoKi::GenChild::Selector& parent       ,
                                       const bool                     mother       )
  : m_first        ( daughter )
  , m_second       ( parent   )
  , m_mother       ( mother   )
{
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
}
// ============================================================================
// constructor from two trees
// ============================================================================
PolarizationAngle::PolarizationAngle ( const Decays::IGenDecay::iTree& daughter ,
                                       const Decays::IGenDecay::iTree& parent   ,
                                       const bool                   mother   )
  : m_first        ( daughter )
  , m_second       ( parent   )
  , m_mother       ( mother   )
{
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
}
// ============================================================================
// constructor from two nodes
// ============================================================================
PolarizationAngle::PolarizationAngle ( const Decays::iNode&         daughter  ,
                                       const Decays::iNode&         parent    ,
                                       const bool                   mother    )
  : m_first        ( daughter )
  , m_second       ( parent   )
  , m_mother       ( mother   )
{
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
}
// ============================================================================
// constructor from two cuts
// ============================================================================
PolarizationAngle::PolarizationAngle ( const LoKi::GenTypes::GCuts& daughter  ,
                                       const LoKi::GenTypes::GCuts& parent    ,
                                       const bool                   mother    )
  : m_first        ( daughter )
  , m_second       ( parent   )
  , m_mother       ( mother   )
{
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
}
// ============================================================================
// constructor from two trees
// ============================================================================
PolarizationAngle::PolarizationAngle ( const std::string&           daughter  ,
                                       const std::string&           parent    ,
                                       const bool                   mother    ,
                                       const std::string&           factory   )
  : m_first        ( daughter , factory )
  , m_second       ( parent   , factory )
  , m_mother       ( mother       )
{
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
PolarizationAngle* PolarizationAngle::clone() const
{ return new PolarizationAngle ( *this ) ; }
// ============================================================================
// get the proper decay components
// ==========================================================================
StatusCode
PolarizationAngle::getComponents12 ( const HepMC::GenParticle*             p   ,
                                     Decays::IGenDecay::iTree::Collection& vct ) const
{
  //
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL") ;
    return StatusCode::FAILURE ;  //                                 RETURN
  }
  //
  Assert ( m_first  .valid () , "The first  tree is invalid!" ) ;
  Assert ( m_second .valid () , "The second tree is invalid!" ) ;
  //
  const HepMC::GenParticle* c1 = m_first.child ( p ) ;
  if ( 0 == c1 )
  {
    Error ( "Invalid first  child : '" + m_first.printOut  () + "'") ;
    return StatusCode::FAILURE ;
  }
  vct.push_back ( c1 ) ;
  //
  const HepMC::GenParticle* c2 = m_second.child ( p ) ;
  if ( 0 == c2 )
  {
    Error ( "Invalid second child : '" + m_second.printOut () + "'") ;
    return StatusCode::FAILURE ;
  }
  vct.push_back ( c2 ) ;
  //
  return StatusCode::SUCCESS ;
}
// ==========================================================================
// MANDATORY: the only one essential method
// ==========================================================================
double PolarizationAngle::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL") ;
    return 1.0 ;  //                                 RETURN
  }
  //
  Decays::IGenDecay::iTree::Collection vct ;
  vct.reserve ( 2 ) ;
  StatusCode sc = getComponents12 ( p , vct );

  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'" , sc ) ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 2 != vct.size() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'" , sc ) ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  return m_mother ?
    LoKi::Kinematics::decayAngle
    ( LoKi::LorentzVector ( vct [0] -> momentum () ) ,
      LoKi::LorentzVector ( vct [1] -> momentum () ) ,
      LoKi::LorentzVector ( p       -> momentum () ) ) :
    LoKi::Kinematics::decayAngle
    ( LoKi::LorentzVector ( vct [0] -> momentum () ) ,
      LoKi::LorentzVector ( vct [0] -> momentum () ) +
      LoKi::LorentzVector ( vct [1] -> momentum () ) ,
      LoKi::LorentzVector ( p       -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& PolarizationAngle::fillStream ( std::ostream& s ) const
{
  s << " GCOSPOL( " << m_first << " , " << m_second ;
  //
  if  ( !m_mother )
  { s << " , " ; Gaudi::Utils::toStream ( m_mother , s ) ; }
  //
  return s << " ) " ;
}



// ============================================================================
// constructor from child-selector
// ============================================================================
SinChi::SinChi ( const LoKi::GenChild::Selector& particle1 ,
                 const LoKi::GenChild::Selector& particle2 ,
                 const LoKi::GenChild::Selector& particle3 ,
                 const LoKi::GenChild::Selector& particle4 )
  : PolarizationAngle ( particle1    ,
                        particle2    ,
                        false        )
  , m_tree3   ( particle3 )
  , m_tree4   ( particle4 )
{
  //
  Assert ( m_tree3.valid () , "The third tree is invalid" ) ;
  Assert ( m_tree4.valid () , "The fourh tree is invalid" ) ;
  //
}
// ============================================================================
// constructor from the trees
// ============================================================================
SinChi::SinChi ( const Decays::IGenDecay::iTree& particle1    ,
                 const Decays::IGenDecay::iTree& particle2    ,
                 const Decays::IGenDecay::iTree& particle3    ,
                 const Decays::IGenDecay::iTree& particle4    )
  : PolarizationAngle ( particle1    ,
                        particle2    ,
                        false        )
  , m_tree3   ( particle3 )
  , m_tree4   ( particle4 )
{
  //
  Assert ( m_tree3.valid () , "The third tree is invalid" ) ;
  Assert ( m_tree4.valid () , "The fourh tree is invalid" ) ;
  //
}
// ============================================================================
// constructor from the nodes
// ============================================================================
SinChi::SinChi ( const Decays::iNode& particle1    ,
                 const Decays::iNode& particle2    ,
                 const Decays::iNode& particle3    ,
                 const Decays::iNode& particle4    )
  : PolarizationAngle ( particle1    ,
                        particle2    ,
                        false        )
  , m_tree3   ( particle3 )
  , m_tree4   ( particle4 )
{
  //
  Assert ( m_tree3.valid () , "The third tree is invalid" ) ;
  Assert ( m_tree4.valid () , "The fourh tree is invalid" ) ;
  //
}
// ============================================================================
// constructor from the cuts
// ============================================================================
SinChi::SinChi ( const LoKi::GenTypes::GCuts& particle1 ,
                 const LoKi::GenTypes::GCuts& particle2 ,
                 const LoKi::GenTypes::GCuts& particle3 ,
                 const LoKi::GenTypes::GCuts& particle4 )
  : PolarizationAngle ( particle1    ,
                        particle2    ,
                        false        )
  , m_tree3   ( particle3 )
  , m_tree4   ( particle4 )
{
  //
  Assert ( m_tree3.valid () , "The third tree is invalid" ) ;
  Assert ( m_tree4.valid () , "The fourh tree is invalid" ) ;
  //
}
// ============================================================================
// constructor from the trees
// ============================================================================
SinChi::SinChi ( const std::string& particle1    ,
                 const std::string& particle2    ,
                 const std::string& particle3    ,
                 const std::string& particle4    ,
                 const std::string& factory      )
  : PolarizationAngle ( particle1    ,
                        particle2    ,
                        false        ,
                        factory      )
  , m_tree3   ( particle3 , factory )
  , m_tree4   ( particle4 , factory )
{
  //
  Assert ( m_tree3.valid () , "The third tree is invalid" ) ;
  Assert ( m_tree4.valid () , "The fourh tree is invalid" ) ;
  //
}
// ============================================================================
// get the proper decay components
// ==========================================================================
StatusCode
SinChi::getComponents34 ( const HepMC::GenParticle*              p   ,
                          Decays::IGenDecay::iTree::Collection& vct ) const
{
  //
  if ( 0 == p )
  {
    Error ( "LHCb::GenParticle* points to NULL") ;
    return StatusCode::FAILURE ;  //                                 RETURN
  }
  //
  Assert ( m_tree3 . valid () , "The third  tree is invalid!" ) ;
  Assert ( m_tree4 . valid () , "The fourth tree is invalid!" ) ;
  //
  const HepMC::GenParticle* c1 = m_tree3.child ( p ) ;
  if ( 0 == c1 )
  {
    Error ( "Invalid third  child : '" + m_tree3.printOut () + "'") ;
    return StatusCode::FAILURE ;
  }
  vct.push_back ( c1 ) ;
  //
  const HepMC::GenParticle* c2 = m_tree4.child ( p ) ;
  if ( 0 == c2 )
  {
    Error ( "Invalid fourth child : '" + m_tree4.printOut () + "'") ;
    return StatusCode::FAILURE ;
  }
  vct.push_back ( c2 ) ;
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// get the proper decay components
// ==========================================================================
StatusCode
SinChi::getComponents ( const HepMC::GenParticle*              p   ,
                        Decays::IGenDecay::iTree::Collection& vct ) const
{
  vct.clear() ;
  StatusCode sc1 = getComponents12  ( p , vct ) ;
  if ( sc1.isFailure() )
  {
    Error ( "Unable to pickup 12-components" , sc1 ) ;
    return sc1 ;
  }
  //
  StatusCode sc2 = getComponents34  ( p , vct ) ;
  if ( sc2.isFailure() )
  {
    Error ( "Unable to pickup 34-components" , sc2 ) ;
    return sc2 ;
  }
  return StatusCode::SUCCESS ;
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
SinChi* SinChi::clone() const { return new SinChi ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double SinChi::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the angle chi:
  return LoKi::Kinematics::sinDecayAngleChi
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& SinChi::fillStream  ( std::ostream& s ) const
{
  s << " GSINCHI( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4  ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================


// ============================================================================
// constructor from selectors
// ============================================================================
CosChi::CosChi ( const LoKi::GenChild::Selector& particle1 ,
                 const LoKi::GenChild::Selector& particle2 ,
                 const LoKi::GenChild::Selector& particle3 ,
                 const LoKi::GenChild::Selector& particle4 )
  : SinChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor form the trees
// ============================================================================
CosChi::CosChi ( const Decays::IGenDecay::iTree& particle1 ,
                 const Decays::IGenDecay::iTree& particle2 ,
                 const Decays::IGenDecay::iTree& particle3 ,
                 const Decays::IGenDecay::iTree& particle4 )
  : SinChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
CosChi::CosChi ( const Decays::iNode& particle1 ,
                 const Decays::iNode& particle2 ,
                 const Decays::iNode& particle3 ,
                 const Decays::iNode& particle4 )
  : SinChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
CosChi::CosChi ( const LoKi::GenTypes::GCuts&   particle1 ,
                 const LoKi::GenTypes::GCuts&   particle2 ,
                 const LoKi::GenTypes::GCuts&   particle3 ,
                 const LoKi::GenTypes::GCuts&   particle4 )
  : SinChi ( particle1 ,
             particle2 ,
             particle3 ,
             particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
CosChi::CosChi ( const std::string& particle1    ,
                 const std::string& particle2    ,
                 const std::string& particle3    ,
                 const std::string& particle4    ,
                 const std::string& factory      )
  : SinChi ( particle1    ,
             particle2    ,
             particle3    ,
             particle4    ,
             factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
CosChi* CosChi::clone() const { return new CosChi ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double CosChi::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the angle chi:
  return LoKi::Kinematics::cosDecayAngleChi
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& CosChi::fillStream  ( std::ostream& s ) const
{
  s << " GCOSCHI( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================


// ============================================================================
// constructor from child-selector
// ============================================================================
AngleChi::AngleChi ( const LoKi::GenChild::Selector& particle1 ,
                     const LoKi::GenChild::Selector& particle2 ,
                     const LoKi::GenChild::Selector& particle3 ,
                     const LoKi::GenChild::Selector& particle4 )
  : CosChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor from the trees
// ============================================================================
AngleChi::AngleChi ( const Decays::IGenDecay::iTree& particle1 ,
                     const Decays::IGenDecay::iTree& particle2 ,
                     const Decays::IGenDecay::iTree& particle3 ,
                     const Decays::IGenDecay::iTree& particle4 )
  : CosChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
AngleChi::AngleChi ( const Decays::iNode& particle1 ,
                     const Decays::iNode& particle2 ,
                     const Decays::iNode& particle3 ,
                     const Decays::iNode& particle4 )
  : CosChi (  particle1 ,
              particle2 ,
              particle3 ,
              particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
AngleChi::AngleChi ( const LoKi::GenTypes::GCuts&   particle1 ,
                     const LoKi::GenTypes::GCuts&   particle2 ,
                     const LoKi::GenTypes::GCuts&   particle3 ,
                     const LoKi::GenTypes::GCuts&   particle4 )
  : CosChi ( particle1 ,
             particle2 ,
             particle3 ,
             particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
AngleChi::AngleChi ( const std::string& particle1    ,
                     const std::string& particle2    ,
                     const std::string& particle3    ,
                     const std::string& particle4    ,
                     const std::string& factory      )
  : CosChi ( particle1    ,
             particle2    ,
             particle3    ,
             particle4    ,
             factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
AngleChi* AngleChi::clone() const { return new AngleChi ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double AngleChi::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the angle chi:
  return LoKi::Kinematics::decayAngleChi
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& AngleChi::fillStream  ( std::ostream& s ) const
{
  s << " GANGLECHI( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================


// ============================================================================
// constructor from child-selector
// ============================================================================
CosThetaTr::CosThetaTr ( const LoKi::GenChild::Selector&  particle1 ,
                         const LoKi::GenChild::Selector&  particle2 ,
                         const LoKi::GenChild::Selector&  particle3 ,
                         const LoKi::GenChild::Selector&  particle4 )
  : AngleChi ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the trees
// ============================================================================
CosThetaTr::CosThetaTr ( const Decays::IGenDecay::iTree&  particle1 ,
                         const Decays::IGenDecay::iTree&  particle2 ,
                         const Decays::IGenDecay::iTree&  particle3 ,
                         const Decays::IGenDecay::iTree&  particle4 )
  : AngleChi ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
CosThetaTr::CosThetaTr ( const Decays::iNode& particle1 ,
                         const Decays::iNode& particle2 ,
                         const Decays::iNode& particle3 ,
                         const Decays::iNode& particle4 )
  : AngleChi ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
CosThetaTr::CosThetaTr ( const LoKi::GenTypes::GCuts&     particle1 ,
                         const LoKi::GenTypes::GCuts&     particle2 ,
                         const LoKi::GenTypes::GCuts&     particle3 ,
                         const LoKi::GenTypes::GCuts&     particle4 )
  : AngleChi ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
CosThetaTr::CosThetaTr ( const std::string& particle1    ,
                         const std::string& particle2    ,
                         const std::string& particle3    ,
                         const std::string& particle4    ,
                         const std::string& factory      )
  : AngleChi ( particle1    ,
               particle2    ,
               particle3    ,
               particle4    ,
               factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
CosThetaTr* CosThetaTr::clone() const { return new CosThetaTr ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double CosThetaTr::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the angle chi:
  return LoKi::Kinematics::cosThetaTr
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream&
CosThetaTr::fillStream  ( std::ostream& s ) const
{
  s << " GCOSTHETATR( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================

// ============================================================================
// constructor from child-selector
// ============================================================================
SinPhiTr::SinPhiTr ( const LoKi::GenChild::Selector&    particle1 ,
                     const LoKi::GenChild::Selector&    particle2 ,
                     const LoKi::GenChild::Selector&    particle3 ,
                     const LoKi::GenChild::Selector&    particle4 )
  : CosThetaTr ( particle1 ,
                 particle2 ,
                 particle3 ,
                 particle4 )
{}
// ============================================================================
// constructor from the trees
// ============================================================================
SinPhiTr::SinPhiTr ( const Decays::IGenDecay::iTree&    particle1 ,
                     const Decays::IGenDecay::iTree&    particle2 ,
                     const Decays::IGenDecay::iTree&    particle3 ,
                     const Decays::IGenDecay::iTree&    particle4 )
  : CosThetaTr ( particle1 ,
                 particle2 ,
                 particle3 ,
                 particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
SinPhiTr::SinPhiTr ( const Decays::iNode& particle1 ,
                     const Decays::iNode& particle2 ,
                     const Decays::iNode& particle3 ,
                     const Decays::iNode& particle4 )
  : CosThetaTr ( particle1 ,
                 particle2 ,
                 particle3 ,
                 particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
SinPhiTr::SinPhiTr ( const LoKi::GenTypes::GCuts&       particle1 ,
                     const LoKi::GenTypes::GCuts&       particle2 ,
                     const LoKi::GenTypes::GCuts&       particle3 ,
                     const LoKi::GenTypes::GCuts&       particle4 )
  : CosThetaTr ( particle1 ,
                 particle2 ,
                 particle3 ,
                 particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
SinPhiTr::SinPhiTr ( const std::string&                 particle1    ,
                     const std::string&                 particle2    ,
                     const std::string&                 particle3    ,
                     const std::string&                 particle4    ,
                     const std::string&                 factory      )
  : CosThetaTr ( particle1    ,
                 particle2    ,
                 particle3    ,
                 particle4    ,
                 factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
SinPhiTr* SinPhiTr::clone() const { return new SinPhiTr ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double SinPhiTr::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the angle chi:
  return LoKi::Kinematics::sinPhiTr
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& SinPhiTr::fillStream  ( std::ostream& s ) const
{
  s << " GSINPHITR( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================

// ============================================================================
// constructor from child-selector
// ============================================================================
CosPhiTr::CosPhiTr ( const LoKi::GenChild::Selector&  particle1 ,
                     const LoKi::GenChild::Selector&  particle2 ,
                     const LoKi::GenChild::Selector&  particle3 ,
                     const LoKi::GenChild::Selector&  particle4 )
  : SinPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the trees
// ============================================================================
CosPhiTr::CosPhiTr ( const Decays::IGenDecay::iTree&  particle1 ,
                     const Decays::IGenDecay::iTree&  particle2 ,
                     const Decays::IGenDecay::iTree&  particle3 ,
                     const Decays::IGenDecay::iTree&  particle4 )
  : SinPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
CosPhiTr::CosPhiTr ( const Decays::iNode&             particle1 ,
                     const Decays::iNode&             particle2 ,
                     const Decays::iNode&             particle3 ,
                     const Decays::iNode&             particle4 )
  : SinPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
CosPhiTr::CosPhiTr ( const LoKi::GenTypes::GCuts&     particle1 ,
                     const LoKi::GenTypes::GCuts&     particle2 ,
                     const LoKi::GenTypes::GCuts&     particle3 ,
                     const LoKi::GenTypes::GCuts&     particle4 )
  : SinPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
CosPhiTr::CosPhiTr ( const std::string&               particle1    ,
                     const std::string&               particle2    ,
                     const std::string&               particle3    ,
                     const std::string&               particle4    ,
                     const std::string&               factory      )
  : SinPhiTr ( particle1    ,
               particle2    ,
               particle3    ,
               particle4    ,
               factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
CosPhiTr* CosPhiTr::clone() const { return new CosPhiTr ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double CosPhiTr::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the cos(phi_tr)
  return LoKi::Kinematics::cosPhiTr
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& CosPhiTr::fillStream  ( std::ostream& s ) const
{
  s << " GCOSPHITR( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================


// ============================================================================
// constructor from child-selector
// ============================================================================
AnglePhiTr::AnglePhiTr ( const LoKi::GenChild::Selector&  particle1 ,
                         const LoKi::GenChild::Selector&  particle2 ,
                         const LoKi::GenChild::Selector&  particle3 ,
                         const LoKi::GenChild::Selector&  particle4 )
  : CosPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the trees
// ============================================================================
AnglePhiTr::AnglePhiTr ( const Decays::IGenDecay::iTree&  particle1 ,
                         const Decays::IGenDecay::iTree&  particle2 ,
                         const Decays::IGenDecay::iTree&  particle3 ,
                         const Decays::IGenDecay::iTree&  particle4 )
  : CosPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the nodes
// ============================================================================
AnglePhiTr::AnglePhiTr ( const Decays::iNode&             particle1 ,
                         const Decays::iNode&             particle2 ,
                         const Decays::iNode&             particle3 ,
                         const Decays::iNode&             particle4 )
  : CosPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor form the cuts
// ============================================================================
AnglePhiTr::AnglePhiTr ( const LoKi::GenTypes::GCuts&     particle1 ,
                         const LoKi::GenTypes::GCuts&     particle2 ,
                         const LoKi::GenTypes::GCuts&     particle3 ,
                         const LoKi::GenTypes::GCuts&     particle4 )
  : CosPhiTr ( particle1 ,
               particle2 ,
               particle3 ,
               particle4 )
{}
// ============================================================================
// constructor from the decay descriptors
// ============================================================================
AnglePhiTr::AnglePhiTr ( const std::string&               particle1    ,
                         const std::string&               particle2    ,
                         const std::string&               particle3    ,
                         const std::string&               particle4    ,
                         const std::string&               factory      )
  : CosPhiTr ( particle1    ,
               particle2    ,
               particle3    ,
               particle4    ,
               factory      )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
AnglePhiTr* AnglePhiTr::clone() const { return new AnglePhiTr ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double AnglePhiTr::operator() ( const HepMC::GenParticle* p ) const
{
  if ( 0 == p )
  {
    Error ( "HepMC::GenParticle* points to NULL, return 'Invaild Angle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  //
  Decays::IGenDecay::iTree::Collection daughters ;
  daughters.reserve ( 4 ) ;
  //
  StatusCode sc = getComponents ( p , daughters ) ;
  //
  if ( sc.isFailure() )
  {
    Error ("Unable to get proper decay components (1), return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  if ( 4 != daughters.size() )
  {
    Error ("Unable to get proper decay components (2) , return 'InvalidAngle'") ;
    return LoKi::Constants::InvalidAngle ;
  }
  // finally evaluate the phi_tr
  return LoKi::Kinematics::anglePhiTr
    ( LoKi::LorentzVector ( daughters [0] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [1] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [2] -> momentum () ) ,
      LoKi::LorentzVector ( daughters [3] -> momentum () ) ) ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& AnglePhiTr::fillStream  ( std::ostream& s ) const
{
  s << " GANGLEPHITR( "
    <<   "'" << child1 ()
    << "','" << child2 ()
    << "','" << child3 ()
    << "','" << child4 ()
    <<   "'" ;
  //
  return s << " ) " ;
}
// ============================================================================

} }

// ============================================================================
// The END
// ============================================================================
