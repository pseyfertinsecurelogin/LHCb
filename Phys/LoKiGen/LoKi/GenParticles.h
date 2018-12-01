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
#ifndef LOKI_GENPARTICLES_H
#define LOKI_GENPARTICLES_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/ParticleID.h"
#include "Kernel/iNode.h"
#include "Kernel/NodesPIDs.h"
#include "LoKi/iTree.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/BasicFunctors.h"
#include "LoKi/Functions.h"
#include "LoKi/GenTypes.h"
#include "LoKi/GenChildSelector.h"
#include "LoKi/PidFunctions.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-23
 *
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::GenParticles GenParticles.h LoKi/GenParticles.h
   *
   *  Collection of LoKi functions and predicates to manipulate with
   *  generator (HepMC) information
   *
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date   2005-03-26
   */
  namespace GenParticles
  {
    // ========================================================================
    /** @class BarCode
     *  the most primitive function - it returns the "barcode"
     *  of HepMC::GenParticle object
     *
     *  @see LoKi::Cuts::GBAR
     *  @see HepMC::GenParticle
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API BarCode : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      BarCode* clone() const override ;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Identifier
     *  primitive function to extract the
     *  particle identifier from HepMC::GenParticle
     *
     *   Algorithm:
     *
     *   int -> ParticleID -> ParticleID::pid()
     *
     *  @see ParticleID
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GID
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API Identifier : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      Identifier* clone() const override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class AbsIdentifier
     *  primitive function to extract the absolute value of
     *  particle identifier from HepMC::GenParticle
     *
     *   Algorithm:
     *
     *   int -> ParticleID -> ParticleID::abspid()
     *
     *  @see ParticleID
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GABSID
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API AbsIdentifier : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      AbsIdentifier* clone() const override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };

    // ========================================================================
    /** @class IsID
     *  new verison of PID-comparison
     */
    // ========================================================================
    struct GAUDI_API IsID  : LoKi::GenTypes::GCuts , LoKi::Pids::GetPids
    {
      // ======================================================================
      IsID ( const long                           id  ) ;
      IsID ( const unsigned long                  id  ) ;
      IsID ( const LHCb::ParticleID&              id  ) ;
      IsID ( const std::string&                   id  ) ;
      IsID ( const std::vector<int>&              ids ) ;
      IsID ( const std::vector<long>&             ids ) ;
      IsID ( const std::vector<unsigned int>&     ids ) ;
      IsID ( const std::vector<unsigned long>&    ids ) ;
      IsID ( const std::vector<LHCb::ParticleID>& ids ) ;
      IsID ( const std::vector<std::string>&      ids ) ;
      IsID ( const LoKi::Pids::GetPids&           ids ) ;
      // ======================================================================
      IsID* clone() const override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class IsNotID
     *  new verison of PID-comparison
     */
    // ========================================================================
    struct GAUDI_API IsNotID : LoKi::GenParticles::IsID
    {
      // ======================================================================
      IsNotID ( const long                           id  ) ;
      IsNotID ( const unsigned long                  id  ) ;
      IsNotID ( const LHCb::ParticleID&              id  ) ;
      IsNotID ( const std::string&                   id  ) ;
      IsNotID ( const std::vector<int>&              ids ) ;
      IsNotID ( const std::vector<long>&             ids ) ;
      IsNotID ( const std::vector<unsigned int>&     ids ) ;
      IsNotID ( const std::vector<unsigned long>&    ids ) ;
      IsNotID ( const std::vector<LHCb::ParticleID>& ids ) ;
      IsNotID ( const std::vector<std::string>&      ids ) ;
      IsNotID ( const LoKi::Pids::GetPids&           ids ) ;
      // ======================================================================
      IsNotID* clone() const override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // ========================================================================
    struct GAUDI_API IsAbsID : LoKi::GenParticles::IsID
    {
      // ======================================================================
      IsAbsID ( const long                           id  ) ;
      IsAbsID ( const unsigned long                  id  ) ;
      IsAbsID ( const LHCb::ParticleID&              id  ) ;
      IsAbsID ( const std::string&                   id  ) ;
      IsAbsID ( const std::vector<int>&              ids ) ;
      IsAbsID ( const std::vector<long>&             ids ) ;
      IsAbsID ( const std::vector<unsigned int>&     ids ) ;
      IsAbsID ( const std::vector<unsigned long>&    ids ) ;
      IsAbsID ( const std::vector<LHCb::ParticleID>& ids ) ;
      IsAbsID ( const std::vector<std::string>&      ids ) ;
      IsAbsID ( const LoKi::Pids::GetPids&           ids ) ;
      // ======================================================================
      IsAbsID* clone() const override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // ========================================================================
    struct GAUDI_API IsNotAbsID : LoKi::GenParticles::IsAbsID
    {
      // ======================================================================
      IsNotAbsID ( const long                           id  ) ;
      IsNotAbsID ( const unsigned long                  id  ) ;
      IsNotAbsID ( const LHCb::ParticleID&              id  ) ;
      IsNotAbsID ( const std::string&                   id  ) ;
      IsNotAbsID ( const std::vector<int>&              ids ) ;
      IsNotAbsID ( const std::vector<long>&             ids ) ;
      IsNotAbsID ( const std::vector<unsigned int>&     ids ) ;
      IsNotAbsID ( const std::vector<unsigned long>&    ids ) ;
      IsNotAbsID ( const std::vector<LHCb::ParticleID>& ids ) ;
      IsNotAbsID ( const std::vector<std::string>&      ids ) ;
      IsNotAbsID ( const LoKi::Pids::GetPids&           ids ) ;
      // ======================================================================
      IsNotAbsID* clone() const override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Status
     *  the most primitive function - it return the "status"
     *  of HepMC::GenParticle object
     *
     *  @see LoKi::Cuts::GTATUS
     *  @see HepMC::GenParticle
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API Status : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      Status* clone() const override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ValidEndVertex
     *  primitive predicate to test the validity of
     *  "end-vertex" for HepMC::GenParticle object
     *
     *  @see HepMC::GenVertex
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GVEV
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API ValidEndVertex : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      ValidEndVertex* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumDistance
     *  Trivial evaluator of euclidian distance
     *  of 4-momentums useful e.g. for trivial
     *  "kinematical matching".
     *  One can find
     *   the particle with "closest" 4-momentum
     *   to the given one:
     *
     *  @code
     *
     *  SEQUENCE            genparticles = ... ;
     *  LoKi::LorentzVector vct       = ... ;
     *
     *  // find "best match"
     *  SEQUENCE::const_iterator imin =
     *    LoKi::select_min( genparticles.begin () ,
     *                      genparticles.end   () ,
     *                      GMOMDIST ( vct )    ) ;
     *
     *  @endcode
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::LorentzVector
     *  @see LoKi::Cuts::GMOMDIST
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    class GAUDI_API MomentumDistance : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /// constructor from 4 components
      MomentumDistance ( const double px ,
                         const double py ,
                         const double pz ,
                         const double e  ) ;
      /** constructor
       *  @param vct the reference 4-momentum
       */
      MomentumDistance ( const LoKi::LorentzVector& vct ) ;
      /// MANDATORY: clone function ("virtual constructor")
      MomentumDistance* clone() const override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::LorentzVector m_vct ;
      // ======================================================================
    };
    // ========================================================================
    /** @class TransverseMomentumRel
     *  Trivial evaluator of the transverse momenrum,
     *  relatve to the given direction
     *
     *  @code
     *
     *  const HepMC::GenParticle*  genparticle = ... ;
     *  LoKi::ThreeVector  vct  = ... ;
     *
     *  GFun pt = GPTDIR( vct ) ;
     *  const double pt = pt ( genparticle ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GPTDIR
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    class GAUDI_API TransverseMomentumRel : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor from theta and phi
       *  @param theta theta angle for the direction
       *  @param phi phi angle for the direction
       */
      TransverseMomentumRel ( const double theta ,
                              const double phi   ) ;
      /** constructor from x,y,z
       *  @param x x-component of the direction vector
       *  @param y y-component of the direction vector
       *  @param z z-component of the direction vector
       */
      TransverseMomentumRel ( const double x ,
                              const double y ,
                              const double z ) ;
      /** constructor from direction vector
       *  @param vct direction vertor
       *  @see LoKi::ThreeVector
       */
      TransverseMomentumRel ( const LoKi::ThreeVector& vct ) ;
      /** constructor from direction vector
       *  @param vct direction vertor
       *  @see LoKi::LorentzVector
       */
      TransverseMomentumRel ( const LoKi::LorentzVector& vct ) ;
      /// MANDATORY: clone function ("virtual constructor")
      TransverseMomentumRel* clone() const override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::ThreeVector m_vct ;
      // ======================================================================
    };
    // ========================================================================
    /** @class FromHepMCTree
     *  simple predicate to test if a certain
     *  HepMC::GenParticle comes from HepMC graph
     *  of another HepMC::GenParticle or vertex
     *
     *
     *  @see LoKi::Cuts::GFROMTREE
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    class GAUDI_API FromHepMCTree : public LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      /** constructor from particle ("head")
       *  @param p pointer to HepMC::GenParticle
       */
      FromHepMCTree
      ( const HepMC::GenParticle* p ) ;
      /// constructor from vector of particles
      FromHepMCTree
      ( const LoKi::GenTypes::GenContainer& r  ) ;
      /// constructor from range of particles
      FromHepMCTree
      ( const LoKi::Types::GRange& r  ) ;
      /** constructor from vertex  ("head")
       *  @param v pointer to HepMC::GenParticle
       */
      FromHepMCTree
      ( const HepMC::GenVertex*   v ) ;
      /// templated constructor
      template <class ITERATOR>
      FromHepMCTree
      ( ITERATOR first , ITERATOR last  )
        : LoKi::GenTypes::GCuts ()
      { _add ( first , last ) ; }
      /// MANDATORY: clone method ("virtual constructor")
      FromHepMCTree* clone() const  override;
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      template <class ITERATOR>
      FromHepMCTree& add ( ITERATOR first , ITERATOR last  )
      { _add ( first , last ) ; return *this ; }
      FromHepMCTree& add ( const HepMC::GenParticle*    p )
      { _add ( p ) ; return *this  ; }
      FromHepMCTree& add ( const HepMC::GenVertex*      p )
      { _add ( p ) ; return *this  ; }
      FromHepMCTree& add ( const LoKi::Types::GRange&   p )
      { _add ( p.begin() , p.end() ) ; return *this  ; }
      FromHepMCTree& remove ( const HepMC::GenVertex*   v ) ;
      FromHepMCTree& remove ( const HepMC::GenParticle* v ) ;
      // ======================================================================
    protected:
      // ======================================================================
      template <class ITERATOR>
      void _add ( ITERATOR first , ITERATOR last  )
      { for ( ; first != last ; ++first ) { _add ( *first ) ; } }
      void _add ( const HepMC::GenParticle* p ) ;
      void _add ( const HepMC::GenVertex*   v ) ;
      // ======================================================================
    private:
      // ======================================================================
      typedef std::vector<HepMC::GenVertex*> VERTICES ;
      VERTICES m_vertices ;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsAnAncestor
     *  simple predicate to test if a certain
     *  HepMC::GenParticle is an ancestor
     *  of another HepMC::GenParticle
     *
     *
     *  @see LoKi::Cuts::GANCESTOR
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    class GAUDI_API IsAnAncestor : public LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      /** constructor from particle
       *  @param p pointer to HepMC::GenParticle
       */
      IsAnAncestor ( const HepMC::GenParticle* p ) ;
      /// MANDATORY: clone method ("virtual constructor")
      IsAnAncestor* clone() const  override;
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      const HepMC::GenParticle* m_particle ;
      // ======================================================================
    };
    // ========================================================================
    /** @class HasQuark
     *  simple predicate , return true if particle has quark 'quark'
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    class  GAUDI_API HasQuark : public LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      HasQuark (  const LHCb::ParticleID::Quark quark ) ;
      /// clone method (mandatory!)
      HasQuark* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      LHCb::ParticleID::Quark quark() const { return m_quark ; }
      // ======================================================================
    private :
      // ======================================================================
      LHCb::ParticleID::Quark m_quark ;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsCharged
     *  simple predicate , return true of particle is charged
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsCharged : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// clone method (mandatory!)
      IsCharged* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsNeutral
     *  simple predicate , return true of particle is neutral
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsNeutral : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// clone method (mandatory!)
      IsNeutral* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsLepton
     *  simple predicate , return true if the particle is a lepton
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsLepton : LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsLepton* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsMeson
     *  simple predicate , return true if particle is a meson
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsMeson : LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsMeson* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsBaryon
     *  simple predicate , return true if the particle is a baryon
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsBaryon : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// clone method (mandatory!)
      IsBaryon* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsHadron
     *  simple predicate , return true of particle is hadron
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    struct GAUDI_API IsHadron : LoKi::GenTypes::GCuts
    {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsHadron* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsNucleus
     *  simple predicate , return true of particle is nucleus
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    struct GAUDI_API IsNucleus : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// clone method (mandatory!)
      IsNucleus* clone() const  override;
      /// the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ProperLifeTime
     *  simple function which evaluate the
     *  proper lifetime (c*tau) of the particle in HepMC units
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    class GAUDI_API ProperLifeTime : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor
       *  @param bad the valut toi be returned for particles
       *  where the evaluation end vertex is not possible
       */
      ProperLifeTime( const double bad = LoKi::Constants::InfiniteTime ) ;
      /// clone method (mandatory!)
      ProperLifeTime* clone() const  override;
      /// the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      double m_bad ;
      // ======================================================================
    };
    // ========================================================================
    /** @class NominalLifeTime
     *  (c*tau) evaluator of the nominal lifetime of the particle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    struct GAUDI_API NominalLifeTime : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// clone method (mandatory!)
      NominalLifeTime* clone() const  override;
      /// the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AdapterToProductionVertex
     *
     *  Simple adapter, which delegates the evaluation of
     *  "vertex" funtion to "production vertex" of the particle:
     *
     *  @see LoKi::Cuts::GFAPVX
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    class GAUDI_API AdapterToProductionVertex : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor from vertex function and "bad" value
       *  @param fun verted function to be used
       *  @param bad the value to be returnedd for invalid vertex
       */
      AdapterToProductionVertex
      ( const LoKi::Types::GVFunc& fun ,
        const double               bad ) ;
      /// clone method (mandatory!)
      AdapterToProductionVertex* clone() const  override;
      /// the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Types::GVFun    m_fun ;
      double                m_bad ;
      // ======================================================================
    };
    // ========================================================================
    /** @class AdapterToEndVertex
     *
     *  Simple adapter, which delegates the evaluation of
     *  "vertex" funtion to "end vertex" of the particle:
     *
     *  @see LoKi::Cuts::GFAEVX
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    class GAUDI_API AdapterToEndVertex : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor from vertex function and "bad" value
       *  @param fun verted function to be used
       *  @param bad the value to be returnedd for invalid vertex
       */
      AdapterToEndVertex
      ( const LoKi::Types::GVFunc& fun ,
        const double               bad ) ;
      /// clone method (mandatory!)
      AdapterToEndVertex* clone() const  override;
      /// the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Types::GVFun    m_fun ;
      double                m_bad ;
      // ======================================================================
    };
    // ========================================================================
    /** @class ThreeCharge
     *  primitive function to extract the 3*charge from HepMC::GenParticle
     *
     *   Algorithm:
     *
     *   pdg_id -> LHCb::ParticleID -> threeCharge
     *
     *  @see ParticleID
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::G3Q
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-03-07
     */
    struct GAUDI_API ThreeCharge : LoKi::GenTypes::GFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      ThreeCharge* clone() const override { return new ThreeCharge(*this) ; }
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class NInTree
     *  simple evaluator of number of particles in the tree
     *  which satisfy a certain predicate
     *
     *  @warning current implementation is valid only for
     *       HepMC::parents, HepMC::children,
     *       HepMC::ancestors and HepMC::descendants
     *
     *  @code
     *
     *  // get Generator information
     *  const HepMCEvents* events =
     *          get<HepMCEvents>( HepMCEventLocation::Default ) ;
     *
     *  typedef std::vector<const HepMC::GenParticle*> GenParticles ;
     *
     *  // select b(and antib) quarks from decay of higgs
     *  GenParticles bquarks ;
     *  LoKi::Extract::genParticles
     *   ( events                         ,
     *     std::back_inserter( bquarks )  ,
     *     ( "b" == GABSID ) &&
     *    1 == GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode
     *
     *  @see HepMC::IteratorRange
     *  @see HepMC::parents
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Extract::getParticles
     *  @see LoKi::Cuts::GNINTREE
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    class GAUDI_API NInTree : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor
       *  @param cut    predicate to be used for counting
       *  @param range  "iterator range", see HepMC::IteratorRange
       *  @see HepMC::IteratorRange
       */
      NInTree ( const LoKi::GenTypes::GCuts& cut                        ,
                HepMC::IteratorRange         range = HepMC::descendants ) ;
      /// MANDATORY: clone function ("virtual constructor")
      NInTree* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    protected:
      // ======================================================================
      /** count the particles in the tree according
       *  the predicat eand iterator range
       *  @see HepMC::GenVertex
       *  @param vertex  root of the tree
       *  @return number of particles
       */
      size_t count( HepMC::GenVertex* vertex ) const ;
      // ======================================================================
    protected: 
      // ======================================================================
      LoKi::GenTypes::GCut  m_cut   ;
      HepMC::IteratorRange  m_range ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class MaxTree
     *  simple evaluator maximal valeu of certain functor in the tree
     *
     *  @warning current implementation is valid only for
     *       HepMC::parents, HepMC::children,
     *       HepMC::ancestors and HepMC::descendants
     *
     *  @see HepMC::IteratorRange
     *  @see HepMC::parents
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Extract::getParticles
     *  @see LoKi::Cuts::GMAXTREE
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-06-12
     */
    class GAUDI_API MaxTree : public NInTree
    {
    public:
      // ======================================================================
      /** constructor
       *  @param cut    predicate to be used for counting
       *  @param range  "iterator range", see HepMC::IteratorRange
       *  @see HepMC::IteratorRange
       */
      MaxTree ( const LoKi::GenTypes::GFunc& fun                        ,
                const LoKi::GenTypes::GCuts& cut                        ,
                HepMC::IteratorRange         range = HepMC::descendants ) ;
      /// MANDATORY: clone function ("virtual constructor")
      MaxTree* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    protected :
      // ======================================================================
      LoKi::GenTypes::GFun  m_fun   ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class MinTree
     *  simple evaluator minimal value of certain functor in the tree
     *
     *  @warning current implementation is valid only for
     *       HepMC::parents, HepMC::children,
     *       HepMC::ancestors and HepMC::descendants
     *
     *  @see HepMC::IteratorRange
     *  @see HepMC::parents
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Extract::getParticles
     *  @see LoKi::Cuts::GMINTREE
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-06-12
     */
    class GAUDI_API MinTree : public MaxTree
    {
    public:
      // ======================================================================
      /** constructor
       *  @param cut    predicate to be used for counting
       *  @param range  "iterator range", see HepMC::IteratorRange
       *  @see HepMC::IteratorRange
       */
      MinTree ( const LoKi::GenTypes::GFunc& fun                        ,
                const LoKi::GenTypes::GCuts& cut                        ,
                HepMC::IteratorRange         range = HepMC::descendants ) ;
      /// MANDATORY: clone function ("virtual constructor")
      MinTree* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class SumTree
     *  simple evaluato of sum over  certain functor values in the tree
     *
     *  @warning current implementation is valid only for
     *       HepMC::parents, HepMC::children,
     *       HepMC::ancestors and HepMC::descendants
     *
     *  @see HepMC::IteratorRange
     *  @see HepMC::parents
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Extract::getParticles
     *  @see LoKi::Cuts::GSUMTREE
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-06-12
     */
    class GAUDI_API SumTree : public MinTree
    {
    public:
      // ======================================================================
      /** constructor
       *  @param cut    predicate to be used for counting
       *  @param range  "iterator range", see HepMC::IteratorRange
       *  @see HepMC::IteratorRange
       */
      SumTree ( const LoKi::GenTypes::GFunc& fun                        ,
                const LoKi::GenTypes::GCuts& cut                        ,
                HepMC::IteratorRange         range = HepMC::descendants ) ;
      /// MANDATORY: clone function ("virtual constructor")
      SumTree* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class MultTree
     *  simple evaluator of the product over  certain functor values in the tree
     *
     *  @warning current implementation is valid only for
     *       HepMC::parents, HepMC::children,
     *       HepMC::ancestors and HepMC::descendants
     *
     *  @see HepMC::IteratorRange
     *  @see HepMC::parents
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Extract::getParticles
     *  @see LoKi::Cuts::GMULTTREE
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-06-12
     */
    class GAUDI_API MultTree : public SumTree
    {
    public:
      // ======================================================================
      /** constructor
       *  @param cut    predicate to be used for counting
       *  @param range  "iterator range", see HepMC::IteratorRange
       *  @see HepMC::IteratorRange
       */
      MultTree ( const LoKi::GenTypes::GFunc& fun                        ,
                 const LoKi::GenTypes::GCuts& cut                        ,
                 HepMC::IteratorRange         range = HepMC::descendants ) ;
      /// MANDATORY: clone function ("virtual constructor")
      MultTree* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class InTree
     *  The trivial predicate which evaluates to true
     *  if there is at least one particle in the decay
     *  tree of the given particle which satisfies the
     *  certain criteria
     *
     *  The function uses the algorithm LoKi::GenAlgs::found
     *
     *  @see LoKi::GenAlgs::found
     *  @see LoKi::Cuts::GINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API InTree : public LoKi::GenTypes::GCuts
    {
    public:
      /** standard constructor
       *  @param cut cut to be checked
       */
      InTree  ( const LoKi::GenTypes::GCuts& cut ) ;
      /// MANDATORY: clone function ("virtual constructor")
      InTree*       clone() const  override;
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
    private:
      // ======================================================================
      /// the criteria itself:
      LoKi::GenTypes::GCut m_cut ;                       // the criteria itself
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Oscillated
     *  Simple class to check the oscillation of the particle
     *  It evaluates to "true" for both particles
     *  @see LoKi::GenParticles::oscillated1
     *  @see LoKi::GenParticles::oscillated2
     *  @see LoKi::Cuts::GOSCILLATED
     *  @author Vanya BELYAEV Ivan.Belyaev@nkhef.nl
     *  @date 2008-07-03
     */
    struct GAUDI_API Oscillated : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Oscillated* clone() const  override;
      /// MANDATORY: the only one essential method
      bool   operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Oscillated1
     *  Simple class to check the oscillation of the particle
     *  @see LoKi::GenParticles::oscillated1
     *  @see LoKi::Cuts::GOSCILLATED1
     *  @author Vanya BELYAEV Ivan.Belyaev@nkhef.nl
     *  @date 2008-07-03
     */
    struct Oscillated1 : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Oscillated1* clone() const  override;
      /// MANDATORY: the only one essential method
      bool   operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Oscillated2
     *  Simple class to check the oscillation of the particle
     *  @see LoKi::GenParticles::oscillated2
     *  @see LoKi::Cuts::GOSCILLATED2
     *  @author Vanya BELYAEV Ivan.Belyaev@nkhef.nl
     *  @date 2008-07-03
     */
    struct GAUDI_API Oscillated2 : LoKi::GenTypes::GCuts
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Oscillated2* clone() const  override;
      /// MANDATORY: the only one essential method
      bool   operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class DecNode
     *  simple predicate, which acts on the particleID of the particle
     *  @see LHCb::ParticleID
     *  @see Decays::iNode
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    class GAUDI_API DecNode
      : public LoKi::BasicFunctors<const HepMC::GenParticle*>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the actual node
      DecNode ( const Decays::iNode& node ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DecNode* clone() const override { return new DecNode ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
      /// get the decay node
      const Decays::iNode& node() const { return m_node.node () ; }
      /// valid node ?
      bool valid() const { return m_node.valid() ; }
      // validate the node
      StatusCode validate ( const LHCb::IParticlePropertySvc* svc ) const
      { return m_node.validate ( svc ) ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the decay node itself
      Decays::Node m_node ;                            // the decay node itself
      // ======================================================================
    };
    // ========================================================================
    /** @class LongLived
     *  simple predicate that select long-lived particles
     *  @see LoKi::Cuts::GLOONGLIVED
     *  @see LHCb::ParticleID
     *  @see Decays::iNode
     *  @see Decays::Nodes::LongLived_
     *  @see LoKi::GenParticles::DecNode
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2016-06-21
     */
    struct GAUDI_API LongLived : DecNode
    {
      // ======================================================================
      /// constructor
      LongLived () : DecNode ( Decays::Nodes::LongLived_() )  {}
      /// MANDATORY: clone method ("virtual constructor")
      LongLived* clone() const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class DecTree
     *  simple predicate, which acts on the decay structuire/tree for the particle
     *  @see Decays::iTree
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-12-17
     */
    class GAUDI_API DecTree
      : public LoKi::BasicFunctors<const HepMC::GenParticle*>::Predicate
    {
    public:
      // ======================================================================
      /// the actual type of tree (interface
      typedef Decays::iTree_<const HepMC::GenParticle*> iTree ; // the tree
      // ======================================================================
    private:
      // ======================================================================
      /// the actual type of tree (assignable)
      typedef Decays::Tree_<const HepMC::GenParticle*>   Tree ; // the tree
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the actual node
      DecTree ( const iTree& node , const bool autovalidate = true ) ;
      /// constructor from the decay descriptor
      DecTree ( const std::string& descriptor ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DecTree* clone() const override { return new DecTree ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the decay node
      const iTree& tree() const { return m_tree.tree () ; }
      // ======================================================================
      /// valid tree ?
      bool valid () const { return m_tree.valid() ; }
      // validate the teh tree
      StatusCode validate ( const LHCb::IParticlePropertySvc* svc ) const
      { return m_tree.validate ( svc ) ; }
      // reset the collection
      void reset() const { m_tree.reset() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the decay tree itself
      Tree m_tree ;                                    // the decay tree itself
      //
      bool m_autovalidate ;
      // ======================================================================
    };
    // ========================================================================
    /** @class ChildFun
     *  adapter to child-function
     *  @see LoKi::Cuts::GCHILD
     *  @see LoKi::Cuts::GCHILDFUN
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-12-11
     */
    class GAUDI_API ChildFun
      : public LoKi::BasicFunctors<const HepMC::GenParticle*>::Function
    {
    public:
      ChildFun
      ( const LoKi::GenTypes::GFunc&    fun       ,
        const LoKi::GenChild::Selector& selector  ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildFun
      ( const LoKi::GenTypes::GFunc& fun       ,
        const std::string&           selector  ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildFun
      ( const LoKi::GenTypes::GFunc&    fun      ,
        const Decays::IGenDecay::iTree& selector ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildFun
      ( const LoKi::GenTypes::GFunc&  fun      ,
        const Decays::iNode&          selector ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildFun
      ( const LoKi::GenTypes::GFunc& fun      ,
        const LoKi::GenTypes::GCuts& selector ) ;
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      ChildFun*  clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL:  specific printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      /// the function itself
      LoKi::GenTypes::GFun     m_fun   ;                 // the function itself
      /// the child selector
      LoKi::GenChild::Selector m_child ;                 //  the child selector
      // ======================================================================
    };
    // ========================================================================
    /** @class ChildCut
     *  adapter to child-predicate
     *  @see LoKi::Cuts::GCHILDCUT
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-12-11
     */
    class GAUDI_API ChildCut
      : public LoKi::BasicFunctors<const HepMC::GenParticle*>::Predicate
    {
    public:
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildCut
      ( const LoKi::GenTypes::GCuts&    fun       ,
        const LoKi::GenChild::Selector& selector  ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildCut
      ( const LoKi::GenTypes::GCuts& fun       ,
        const std::string&           selector  ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildCut
      ( const LoKi::GenTypes::GCuts&    fun      ,
        const Decays::IGenDecay::iTree& selector ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildCut
      ( const LoKi::GenTypes::GCuts&  fun      ,
        const Decays::iNode&          selector ) ;
      /** constructor from the function and child selector
       *  @param fun      the function to be used
       *  @param selector the child selector
       */
      ChildCut
      ( const LoKi::GenTypes::GCuts&  fun      ,
        const LoKi::GenTypes::GCuts&  selector ) ;
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      ChildCut*  clone() const  override;
      /// MANDATORY: the only one essential method
      bool operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL:  specific printout
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      /// the function itself
      LoKi::GenTypes::GCut     m_fun   ;                 // the function itself
      /// the child selector
      LoKi::GenChild::Selector m_child ;                 //  the child selector
      // ======================================================================
    };
    // ========================================================================
  } //                                      end of namespace LoKi::GenParticles
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENPARTICLES_H
// ============================================================================
