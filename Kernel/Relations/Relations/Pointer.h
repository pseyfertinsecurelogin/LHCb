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
#ifndef RELATIONS_POINTER_H
#define RELATIONS_POINTER_H 1
// ============================================================================
// Include files
// ============================================================================

namespace Relations {
  /** @class Pointer Pointer.h Relations/Pointer.h
   *
   *  simple wrapper class for pointer ( similar to boost::ptr)
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
   *  @date   2005-05-12
   */
  template <class TYPE>
  class Pointer {
  public:
    Pointer( const TYPE* data = nullptr ) : m_data( const_cast<TYPE*>( data ) ){};
    /// destructor
    ~Pointer() = default;

  public:
    TYPE* get() const { return m_data; }
          operator TYPE*() const { return get(); }
    TYPE* operator->() const { return get(); }

  public:
    Pointer& operator=( const TYPE* value ) {
      m_data = const_cast<TYPE*>( value );
      return *this;
    };

  public:
    bool operator<( const Pointer& right ) const { return m_data < right.m_data; }
    bool operator==( const Pointer& right ) const { return &right == this || m_data == right.m_data; }

  private:
    TYPE* m_data;
  };
  // "const-away"
  template <class TYPE>
  class Pointer<const TYPE> : public Pointer<TYPE> {
  public:
    /// constructor
    Pointer( const TYPE* data = nullptr ) : Pointer( data ) {}
    /// destructor
    ~Pointer() = default;
  };

} // end of namespace Relations

// ============================================================================
//  The END
// ============================================================================
#endif // RELATIONS_POINTER_H
