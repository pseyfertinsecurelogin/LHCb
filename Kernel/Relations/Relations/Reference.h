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
#ifndef RELATIONS_REFERENCE_H
#define RELATIONS_REFERENCE_H 1
// ============================================================================
// Include files
// ============================================================================
// ST D& STL
// ============================================================================
#include <cassert>
#include <functional>
// ============================================================================

namespace Relations {
  /** @class Reference Reference.h Relations/Reference.h
   *
   *
   *  @author Vanya BELYAEV ibelyaev@physcis.syr.edu
   *  @date   2006-06-09
   */
  template <class TYPE>
  class Reference {
  public:
    /// Standard constructor
    Reference( TYPE& data ) : m_data( &data ){};
    /// destructor
    ~Reference() = default;

  public:
    TYPE& get() const {
      assert( 0 != m_data && "Relations::Reference:get(): invalid pointer" );
      return *m_data;
    };
          operator TYPE&() const { return get(); }
    TYPE& operator*() const { return get(); }

  public:
    Reference& operator=( const TYPE& value ) {
      m_data = const_cast<TYPE*>( &value );
      return *this;
    };

  public:
    bool operator<( const Reference& right ) const {
      const std::less<> _less;
      return &right == this ? false
                            : m_data == right.m_data
                                  ? false
                                  : 0 == m_data ? true : 0 == right.m_data ? false : _less( *m_data, *right.m_data );
    };
    bool operator==( const Reference& right ) const {
      const std::equal_to<> _equal;
      return &right == this ? true
                            : m_data == right.m_data
                                  ? true
                                  : 0 == m_data ? false : 0 == right.m_data ? false : _equal( *m_data, *right.m_data );
    }

  private:
    TYPE* m_data = nullptr;
  };
} // namespace Relations

// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_REFERENCE_H
