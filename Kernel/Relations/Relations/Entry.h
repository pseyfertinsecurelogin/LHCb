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
#ifndef RELATIONS_ENTRY_H
#define RELATIONS_ENTRY_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
#include <utility>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/ObjectTypeTraits.h"
// ============================================================================
namespace Relations {
  // ==========================================================================
  /** @struct Entry_
   *  An auxillary class to implement the relations
   *  @see std::pair
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   27/01/2002
   */
  template <class FROM, class TO>
  struct Entry_ {
  public:
    /// type traits for "FROM" object
    using FromTypes = Relations::ObjectTypeTraits<FROM>;
    /// type traits for "TO" object
    using ToTypes = Relations::ObjectTypeTraits<TO>;

  public:
    using From_     = typename FromTypes::Input;
    using From      = typename FromTypes::Output;
    using FromInner = typename FromTypes::Inner;
    using To_       = typename ToTypes::Input;
    using To        = typename ToTypes::Output;
    using ToInner   = typename ToTypes::Inner;

  public:
    /// "less" function object for "From" objects
    using LessF = typename FromTypes::Less;
    /// "less" function object for "To" objects
    using LessT = typename ToTypes::Less;
    /// "equality" function object for "To" objects
    using EqualT = typename std::equal_to<>;

  public:
    using First  = FromInner;
    using Second = ToInner;

  public:
    /// constructor
    Entry_() = default;
    /// constructor
    Entry_( From_ f, To_ t ) : m_from( f ), m_to( t ) {}
    /// constructor
    Entry_( From_ f ) : m_from( f ) {}

  public:
    /// accessor to the "FROM" object ( const     version )
    From from() const { return m_from; }
    /// accessor to the "TO"   object ( const     version )
    To to() const { return m_to; }
    /// the conversion operator       ( const     version )
    operator To() const { return m_to; }

  public:
    /// for Python @warning  "from" is reserved keyword in Python
    From _from() const { return from(); }
    /// for Python
    To _to() const { return to(); }

  public:
    /// comparison operator
    bool operator<( const Entry_& entry ) const {
      return LessF()( this->m_from, entry.m_from )
                 ? true
                 : LessF()( entry.m_from, this->m_from ) ? false : LessT()( this->m_to, entry.m_to );
    }

  public:
    mutable FromInner m_from;
    mutable ToInner   m_to;
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_ENTRY_H
