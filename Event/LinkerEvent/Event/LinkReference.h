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
#ifndef LinkerEvent_LinkReference_H
#define LinkerEvent_LinkReference_H 1

// Include files
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class LinkReference LinkReference.h
   *
   * Entry to get a reference to a keyed object
   *
   * @author Olivier Callot
   * created Thu Mar 28 08:35:26 2019
   *
   */

  class LinkReference final {
  public:
    /// Constructor with arguments
    LinkReference( int srcLinkID, int linkID, int key, int nextIndex, float weight );

    /// Default Constructor
    LinkReference() : m_srcLinkID( 0 ), m_linkID( 0 ), m_objectKey( 0 ), m_nextIndex( 0 ), m_weight( 0.0 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  index of the source sontainer in the LinkManager
    short srcLinkID() const;

    /// Update  index of the source sontainer in the LinkManager
    void setSrcLinkID( short value );

    /// Retrieve const  index of the target container in the LinkManager
    short linkID() const;

    /// Update  index of the target container in the LinkManager
    void setLinkID( short value );

    /// Retrieve const  key of the object in the container
    int objectKey() const;

    /// Update  key of the object in the container
    void setObjectKey( int value );

    /// Retrieve const  index of the next entry
    int nextIndex() const;

    /// Update  index of the next entry
    void setNextIndex( int value );

    /// Retrieve const  weight of the reference
    float weight() const;

    /// Update  weight of the reference
    void setWeight( float value );

    friend std::ostream& operator<<( std::ostream& str, const LinkReference& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    short m_srcLinkID; ///< index of the source sontainer in the LinkManager
    short m_linkID;    ///< index of the target container in the LinkManager
    int   m_objectKey; ///< key of the object in the container
    int   m_nextIndex; ///< index of the next entry
    float m_weight;    ///< weight of the reference

  }; // class LinkReference

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::LinkReference::LinkReference( int srcLinkID, int linkID, int key, int nextIndex, float weight ) {

  m_srcLinkID = short( srcLinkID );
  m_linkID    = short( linkID );
  m_objectKey = key;
  m_nextIndex = nextIndex;
  m_weight    = weight;
}

inline std::ostream& LHCb::LinkReference::fillStream( std::ostream& s ) const {
  s << "{ "
    << "srcLinkID :	" << m_srcLinkID << std::endl
    << "linkID :	" << m_linkID << std::endl
    << "objectKey :	" << m_objectKey << std::endl
    << "nextIndex :	" << m_nextIndex << std::endl
    << "weight :	" << m_weight << std::endl
    << " }";
  return s;
}

inline short LHCb::LinkReference::srcLinkID() const { return m_srcLinkID; }

inline void LHCb::LinkReference::setSrcLinkID( short value ) { m_srcLinkID = value; }

inline short LHCb::LinkReference::linkID() const { return m_linkID; }

inline void LHCb::LinkReference::setLinkID( short value ) { m_linkID = value; }

inline int LHCb::LinkReference::objectKey() const { return m_objectKey; }

inline void LHCb::LinkReference::setObjectKey( int value ) { m_objectKey = value; }

inline int LHCb::LinkReference::nextIndex() const { return m_nextIndex; }

inline void LHCb::LinkReference::setNextIndex( int value ) { m_nextIndex = value; }

inline float LHCb::LinkReference::weight() const { return m_weight; }

inline void LHCb::LinkReference::setWeight( float value ) { m_weight = value; }

#endif /// LinkerEvent_LinkReference_H
