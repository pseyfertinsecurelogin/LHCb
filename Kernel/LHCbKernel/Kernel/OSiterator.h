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
#ifndef KERNEL_OSITERATOR_H
#define KERNEL_OSITERATOR_H 1
// ============================================================================
// STL
#include <iterator>

/** @class OS_iterator OSIterator.h Kernel/OSIterator.h
 *  @brief output stream iterator
 *
 *  a simple helpful class for maintain the notion
 *  of output iterator to arbitrary stream.
 *
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    26/11/1999
 */

template <class TYPE, class OSTREAM = std::ostream, class TERMINATOR = const char*>
class OS_iterator {
  ///
public:
  ///
  using iterator_category = std::output_iterator_tag;
  using value_type        = void;
  using difference_type   = void;
  using pointer           = void;
  using reference         = void;
  ///
  using Type       = TYPE;
  using Terminator = TERMINATOR;
  using Stream     = OSTREAM;
  ///
  typedef OS_iterator<TYPE, OSTREAM, TERMINATOR> MyType;
  using Manipulator = Stream& (*)( Stream& );
  ///
  OS_iterator( OSTREAM& s )
      : stream( &s ), terminator(), manipulator( 0 ), has_terminator( false ), has_manipulator( false ){};
  OS_iterator( OSTREAM& s, Manipulator fun )
      : stream( &s ), terminator(), manipulator( fun ), has_terminator( false ), has_manipulator( true ){};
  OS_iterator( OSTREAM& s, TERMINATOR c )
      : stream( &s ), terminator( c ), manipulator( 0 ), has_terminator( true ), has_manipulator( false ){};
  OS_iterator( OSTREAM& s, TERMINATOR c, Manipulator fun )
      : stream( &s ), terminator( c ), manipulator( fun ), has_terminator( true ), has_manipulator( true ){};
  ///
  MyType& operator=( const TYPE& value ) {
    *stream << value;
    if ( has_terminator ) *stream << terminator;
    if ( has_manipulator && 0 != manipulator ) ( *manipulator )( *stream );
    return *this;
  }
  ///
  MyType& operator*() { return *this; }
  MyType& operator++() { return *this; }
  /// NB !!! It returns the reference also!!
  MyType& operator++( int ) { return *this; }
  ///
protected:
  ///
  Stream*     stream;
  Terminator  terminator;
  Manipulator manipulator;
  bool        has_terminator;
  bool        has_manipulator;
  ///
};

template <class TYPE, class OSTREAM, class TERMINATOR>
inline std::output_iterator_tag iterator_category( const OS_iterator<TYPE, OSTREAM, TERMINATOR>& ) {
  return std::output_iterator_tag();
}

// ============================================================================
// The End
// ============================================================================
#endif ///<  KERNEL_OSITERATOR_H
