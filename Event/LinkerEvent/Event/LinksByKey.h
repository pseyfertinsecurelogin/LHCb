/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef LinkerEvent_LinksByKey_H
#define LinkerEvent_LinksByKey_H 1

// Include files
#include "Event/LinkReference.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_LinksByKey = 400;

  /** @class LinksByKey LinksByKey.h
   *
   * Link of an object to Keyed objects
   *
   * @author OlivierCallot
   * created Fri Mar 22 11:58:15 2019
   *
   */

  class LinksByKey final : public DataObject {
  public:
    /// Default constructor, reserve space
    LinksByKey();

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Define the increasing order for the references
    void setIncreasing();

    /// Define the decreasing order for the references
    void setDecreasing();

    /// Get the pointers to the linked containers
    void resolveLinks( IDataProviderSvc* eventSvc );

    /// Add a reference to the specified key
    void addReference( int srcKey, int srcLinkID, int destKey, int destLinkID, double weight = 1. );

    /// Retrieve the first LinkReference for this key
    bool firstReference( int key, const DataObject* container, LHCb::LinkReference& reference ) const;

    /// Retrieve the next LinkReference for this key
    bool nextReference( LHCb::LinkReference& reference ) const;

    /// Checks whether a given object has any link
    bool hasEntry( const KeyedObject<int>& obj ) const;

    /// Retrieve the first key for this LinkReference
    int firstSource( LHCb::LinkReference& reference, std::vector<std::pair<int, int>>::const_iterator& iter ) const;

    /// Retrieve the next key for this LinkReference
    int nextSource( LHCb::LinkReference& reference, std::vector<std::pair<int, int>>::const_iterator& iter ) const;

    /// Returns the LinkManager linkID for this data object (Container)
    int linkID( const DataObject* obj ) const;

    /// Reset the content of the objects
    void reset();

    /// Retrieve const  List of linked objects
    const std::vector<std::pair<int, int>>& keyIndex() const;

    /// Retrieve const  List of references
    const std::vector<LHCb::LinkReference>& linkReference() const;

    /// Retrieve const  Class ID of the source of the Link
    unsigned int sourceClassID() const;

    /// Update  Class ID of the source of the Link
    void setSourceClassID( unsigned int value );

    /// Retrieve const  Class ID of the target of the Link
    unsigned int targetClassID() const;

    /// Update  Class ID of the target of the Link
    void setTargetClassID( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const LinksByKey& obj ) { return obj.fillStream( str ); }

    /**
     * applies a given function to all links of the LinksByKey object
     * Function must have the following signature :
     *    (unsigned int srcIndex, unsigned int tgtIndex, float weight)
     * where
     *   - srcindex is the index of the source item in the source container
     *   - tgtindex is the index of the target item in the target container
     *   - weight is the weight of the current link
     */
    template <typename Function>
    void applyToAllLinks( Function&& func ) const;

    /**
     * applies a given function to all links related to given source
     * Function must have the following signature :
     *    (unsigned int srcIndex, unsigned int tgtIndex, float weight)
     * where
     *   - srcindex is the index of the source item in the source container
     *   - tgtindex is the index of the target item in the target container
     *   - weight is the weight of the current link
     */
    template <typename Function>
    void applyToLinks( unsigned int srcIndex, Function&& func ) const;

  protected:
    /// Returns the index of the key in m_keyIndex. True if key exist, else inserting position
    bool findIndex( int key, int& index ) const;

  private:
    /**
     * Internal helper function applying the given function to all links of the
     * chain given by refIndex
     */
    template <typename Function>
    void internalApply( unsigned int srcIndex, int refIndex, Function&& func ) const;

  private:
    bool                             m_increasing;    ///< Type of ordering
    std::vector<std::pair<int, int>> m_keyIndex;      ///< List of linked objects
    std::vector<LHCb::LinkReference> m_linkReference; ///< List of references
    unsigned int                     m_sourceClassID; ///< Class ID of the source of the Link
    unsigned int                     m_targetClassID; ///< Class ID of the target of the Link

  }; // class LinksByKey

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::LinksByKey::LinksByKey() {

  m_keyIndex.reserve( 1000 );
  m_linkReference.reserve( 1000 );
  m_increasing    = false;
  m_targetClassID = 0;
  m_sourceClassID = 0;
}

inline const CLID& LHCb::LinksByKey::clID() const { return LHCb::LinksByKey::classID(); }

inline const CLID& LHCb::LinksByKey::classID() { return CLID_LinksByKey; }

inline std::ostream& LHCb::LinksByKey::fillStream( std::ostream& s ) const {
  char l_increasing = ( m_increasing ) ? 'T' : 'F';
  s << "{ "
    << "increasing :	" << l_increasing << std::endl
    << "keyIndex :	" << m_keyIndex << std::endl
    << "linkReference :	" << m_linkReference << std::endl
    << "sourceClassID :	" << m_sourceClassID << std::endl
    << "targetClassID :	" << m_targetClassID << std::endl
    << " }";
  return s;
}

inline const std::vector<std::pair<int, int>>& LHCb::LinksByKey::keyIndex() const { return m_keyIndex; }

inline const std::vector<LHCb::LinkReference>& LHCb::LinksByKey::linkReference() const { return m_linkReference; }

inline unsigned int LHCb::LinksByKey::sourceClassID() const { return m_sourceClassID; }

inline void LHCb::LinksByKey::setSourceClassID( unsigned int value ) { m_sourceClassID = value; }

inline unsigned int LHCb::LinksByKey::targetClassID() const { return m_targetClassID; }

inline void LHCb::LinksByKey::setTargetClassID( unsigned int value ) { m_targetClassID = value; }

inline void LHCb::LinksByKey::setIncreasing() { m_increasing = true; }

inline void LHCb::LinksByKey::setDecreasing() { m_increasing = false; }

inline void LHCb::LinksByKey::reset() {
  m_keyIndex.clear();
  m_linkReference.clear();
}

template <typename Function>
void LHCb::LinksByKey::internalApply( unsigned int srcIndex, int refIndex, Function&& func ) const {
  while ( 0 <= refIndex ) {
    auto& reference = m_linkReference[refIndex];
    func( srcIndex, (unsigned int)reference.objectKey(), reference.weight() );
    refIndex = reference.nextIndex();
  }
}

template <typename Function>
void LHCb::LinksByKey::applyToAllLinks( Function&& func ) const {
  for ( auto [srcIndex, refIndex] : m_keyIndex ) { internalApply( srcIndex, refIndex, func ); }
}

template <typename Function>
void LHCb::LinksByKey::applyToLinks( unsigned int srcIndex, Function&& func ) const {
  int key;
  if ( findIndex( srcIndex, key ) ) { internalApply( srcIndex, m_keyIndex[key].second, func ); }
}

#endif /// LinkerEvent_LinksByKey_H
