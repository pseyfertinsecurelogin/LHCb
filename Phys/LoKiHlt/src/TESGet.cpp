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
// Event
// ============================================================================
#include "Event/ODIN.h"
#include "Event/L0DUReport.h"
#include "Event/HltDecReports.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/TESGet.h"
// ============================================================================
namespace
{
  // ==========================================================================
  template <class TYPE>
  class _C : public LoKi::Functor<void,bool>
  {
  public:
    // ========================================================================
    _C ( const LoKi::TES::Get&                  obj ,
         const LoKi::Functor<const TYPE*,bool>& cut )
      : m_get ( obj )
      , m_cut ( cut )
    {}
    // ========================================================================
    _C* clone() const override { return new _C(*this) ; }
    // ========================================================================
    std::ostream& fillStream ( std::ostream& s ) const override
    { return  s << " (" << m_get << " >> " << m_cut  << ") " ; }
    //
    std::string toCpp() const override
    {
      return
        "LoKi::TES::TESGet::get( "    +
        Gaudi::Utils::toCpp ( m_get ) + ", " +
        Gaudi::Utils::toCpp ( m_cut ) + ") " ;
    }
    // ========================================================================
    bool operator() (  ) const override
    {
      //
      if ( !m_t || !sameEvent() )
      {
        m_t = LoKi::TES::get_<TYPE> ( m_get ) ;
        setEvent () ;
      }
      //
      if ( !m_t )
      {
        Error ( "No valid object is found at '" + m_get.location() + "', return False") ;
        return false ;
      }
      //
      return m_cut ( m_t ) ;
    }
    // ========================================================================
  private:
    // ========================================================================
    LoKi::TES::Get                             m_get ;
    LoKi::FunctorFromFunctor<const TYPE*,bool> m_cut ;
    mutable const TYPE*                        m_t   = nullptr;
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  class _F : public LoKi::Functor<void,double>
  {
  public:
    // ========================================================================
    _F ( const LoKi::TES::Get&                    obj ,
         const LoKi::Functor<const TYPE*,double>& fun )
      : m_get ( obj )
      , m_fun ( fun )
    {}
    // ========================================================================
    _F* clone() const override { return new _F(*this) ; }
    // ========================================================================
    std::ostream& fillStream ( std::ostream& s ) const override
    { return  s << " (" << m_get << " >> " << m_fun  << ") " ; }
    // ========================================================================
    std::string toCpp() const override
    {
      return "LoKi::TES::TESGet::get( "    +
             Gaudi::Utils::toCpp ( m_get ) + ", " +
             Gaudi::Utils::toCpp ( m_fun ) + ") " ;
    }
    // ========================================================================
    double operator() (  ) const override
    {
      //
      if ( !m_t || !sameEvent() )
      {
        m_t = LoKi::TES::get_<TYPE> ( m_get ) ;
        setEvent () ;
      }
      //
      if ( !m_t )
      {
        Error ( "No valid object is found at '" + m_get.location() + "', return -inf") ;
        return false ;
      }
      //
      return m_fun ( m_t ) ;
    }
    // ========================================================================
  private:
    // ========================================================================
    LoKi::TES::Get                               m_get ;
    LoKi::FunctorFromFunctor<const TYPE*,double> m_fun ;
    mutable const TYPE*                          m_t  = nullptr;
    // ========================================================================
  };
  // ==========================================================================
} // end of anonymous namespace
// ============================================================================
LoKi::FunctorFromFunctor<void,bool>
LoKi::TES::TESGet::get( const LoKi::TES::Get&                          obj ,
                        const LoKi::Functor<const LHCb::ODIN*,bool>&   cut )
{ return _C<LHCb::ODIN>( obj , cut ) ; }
// ============================================================================
LoKi::FunctorFromFunctor<void,double>
LoKi::TES::TESGet::get( const LoKi::TES::Get&                            obj ,
                        const LoKi::Functor<const LHCb::ODIN*,double>&   fun )
{ return _F<LHCb::ODIN>( obj , fun ) ; }
// ============================================================================
LoKi::FunctorFromFunctor<void,bool>
LoKi::TES::TESGet::get ( const LoKi::TES::Get&                                obj ,
                         const LoKi::Functor<const LHCb::L0DUReport*,bool>&   cut )
{ return _C<LHCb::L0DUReport>( obj , cut ) ; }
// ============================================================================
LoKi::FunctorFromFunctor<void,double>
LoKi::TES::TESGet::get ( const LoKi::TES::Get&                                  obj ,
                         const LoKi::Functor<const LHCb::L0DUReport*,double>&   fun )
{ return _F<LHCb::L0DUReport>( obj , fun ) ; }
// ============================================================================
LoKi::FunctorFromFunctor<void,bool>
LoKi::TES::TESGet::get ( const LoKi::TES::Get&                                   obj ,
                         const LoKi::Functor<const LHCb::HltDecReports*,bool>&   cut )
{ return _C<LHCb::HltDecReports>( obj , cut ) ; }
// ============================================================================
LoKi::FunctorFromFunctor<void,double>
LoKi::TES::TESGet::get ( const LoKi::TES::Get&                                     obj ,
                         const LoKi::Functor<const LHCb::HltDecReports*,double>&   fun )
{ return _F<LHCb::HltDecReports>( obj , fun ) ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
