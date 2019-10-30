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
#ifndef RELATIONS_WENTRY_H
#define RELATIONS_WENTRY_H 1
// ============================================================================
// Include files
// ============================================================================
// Relations
// ============================================================================
#include "Relations/ObjectTypeTraits.h"
// ============================================================================
namespace Relations {
  /** @struct WEntry_
   *
   *
   *  @author Ivan BELYAEV
   *  @date   2006-06-11
   */
  template <class FROM, class TO, class WEIGHT>
  struct WEntry_ {
  public:
    // ========================================================================
    /// type traits for "FROM" object
    using FromTypes = Relations::ObjectTypeTraits<FROM>;
    /// type traits for "TO" object
    using ToTypes = Relations::ObjectTypeTraits<TO>;
    /// type traits for "WEIGHT" object
    using WeightTypes = Relations::ObjectTypeTraits<WEIGHT>;
    // ========================================================================
  public:
    // ========================================================================
    using From_       = typename FromTypes::Input;
    using From        = typename FromTypes::Output;
    using FromInner   = typename FromTypes::Inner;
    using To_         = typename ToTypes::Input;
    using To          = typename ToTypes::Output;
    using ToInner     = typename ToTypes::Inner;
    using Weight_     = typename WeightTypes::Input;
    using Weight      = typename WeightTypes::Output;
    using WeightInner = typename WeightTypes::Inner;
    // ========================================================================
  public:
    // ========================================================================
    /// "less" function object for "From" objects
    using LessF = typename FromTypes::Less;
    /// "less" function object for "To" objects
    using LessT = typename ToTypes::Less;
    /// "less" function object for "Weight" objects
    using LessW = typename WeightTypes::Less;
    /// "equality" function object for "To" objects
    using EqualT = typename std::equal_to<ToInner>;
    // ========================================================================
  public:
    // ========================================================================
    using First  = FromInner;
    using Second = ToInner;
    using Third  = WeightInner;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    WEntry_() = default;
    WEntry_( From_ f ) : m_from( f ){};
    WEntry_( From_ f, To_ t ) : m_from( f ), m_to( t ){};
    WEntry_( From_ f, To_ t, Weight_ w ) : m_from( f ), m_to( t ), m_weight( w ){};
    // ========================================================================
  public:
    // ========================================================================
    /// accessor to the "FROM" object ( const     version )
    From from() const noexcept { return m_from; }
    /// accessor to the "TO"   object ( const     version )
    To to() const noexcept { return m_to; }
    /// accessor to the "WEIGHT"   object (     const version )
    Weight weight() const noexcept { return m_weight; }
    /// the conversion operator       ( const     version )
    operator To() const noexcept { return m_to; }
    // ========================================================================
  public:
    // ========================================================================
    /// for Python @warning  "from" is reserved keyword in Python
    From _from() const noexcept { return from(); }
    /// for Python
    To _to() const noexcept { return to(); }
    /// for Python
    Weight _weight() const noexcept { return weight(); }
    // ========================================================================
  public:
    // ========================================================================
    /// comparison operator
    bool operator<( const WEntry_& entry ) const noexcept {
      static const LessF _lessF = LessF();
      static const LessW _lessW = LessW();
      return ( _lessF( this->m_from, entry.m_from )
                   ? true
                   : _lessF( entry.m_from, this->m_from ) ? false : _lessW( this->m_weight, entry.m_weight ) );
    }
    // ========================================================================
  public:
    // ========================================================================
    mutable FromInner   m_from   = {};
    mutable ToInner     m_to     = {};
    mutable WeightInner m_weight = {};
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_WENTRY_H
