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
#ifndef KERNEL_CALOCELLIDKEYTRAITS_H
#define KERNEL_CALOCELLIDKEYTRAITS_H 1
// Include files
// Kernel
#include "Kernel/CaloCellID.h"
// EventKernel
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/KeyedTraits.h"

/** @file CaloCellIDKeyTraits.h
 *
 *  The mandatory specializations to use CaloCellID class as a KEY
 *  for containers with KEYED access
 *
 *  The specialisation include:
 *
 *   - the specialization of  Containers::key_traits structure
 *
 *   - the specialization of serializers for class KeyedObject
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   15/03/2002
 */
namespace Containers {
  /** Function to be called at each attempt of automatic creation of CaloCellID
   *  object as a KEY for KeyedObjects
   *
   *  @exception GaudiException
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date 20/03/2002
   */
  void errorMakeCaloCellIDKey();

  /** the template specialisation of general
   *  Containers::key_traits<KEY> structure for the case of
   *  KEY = CaloCellID
   *
   *  One need such specialiation to provide
   *  conversion form the CaloCellID to long
   *  representation ("identifier")
   *  and to index representation ("hash")
   *  And to forbid the automatic assignement of key
   *
   *  @see CaloCellID
   *  @see Containers::key_traits
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   15/03/2002
   */
  template <>
  struct key_traits<LHCb::CaloCellID> {
    /// Declaration of key-type
    using key_type = LHCb::CaloCellID;
    /// Declaration of keyed object type
    using obj_type = KeyedObject<key_type>;

    /** Create key from its full integer representation.
     *  Not implementing on specialization may inhibit the creation
     *  of keys, i.e. then a key must be supplied at insertion time.
     */
    static key_type makeKey( long k ) {
      // errorMakeCaloCellIDKey();
      key_type _key;
      _key.setAll( k );
      return _key;
    };

    /// Full unhashed key identifier
    static long identifier( const key_type& k ) { return k.all(); }

    /// Hash function for this key
    static long hash( const key_type& k ) { return k.index(); }

    /// Set object key when inserted into the container
    static void setKey( obj_type* v, const key_type& k ) {
      if ( v ) { v->setKey( k ); };
    }

    /** Check the validity of the object's key.
     *  Select if key-checks should be performed by
     *  switching on/off the macro CHECK_KEYED_CONTAINER.
     */
#ifdef CHECK_KEYED_CONTAINER
    static bool checkKey( obj_type* v, const key_type& k ) { return ( v ) ? ( hash( v->key() ) == hash( k ) ) : false; }
#else
    static bool checkKey( obj_type* /* v */, const key_type& /* k */ ) { return true; }
#endif
    /// Add reference counter to object when inserted into the container
    static long addRef( obj_type* v ) { return ( v ) ? v->addRef() : 0; }
    /// Release reference to object
    static long release( obj_type* v ) { return ( v ) ? v->release() : 0; }
  };
} // namespace Containers

// ============================================================================
// The End
// ============================================================================
#endif // KERNEL_CALOCELLIDKEYTRAITS_H
