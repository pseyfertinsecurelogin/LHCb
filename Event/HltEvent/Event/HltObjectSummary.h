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
#pragma once

// Include files
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/LHCbID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HltObjectSummary = 7530;

  // Namespace for locations in TDS
  namespace HltObjectSummaryLocation {
    inline const std::string Default  = "Hlt/SelReports/Candidates";
    inline const std::string Emulated = "Hlt/Emu/SelReports/Candidates";
  } // namespace HltObjectSummaryLocation

  /** @class HltObjectSummary HltObjectSummary.h
   *
   * Summary class for selected Hlt candidates (Particles, Tracks, vertices,
   * etc.), and selection list itself.
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltObjectSummary final : public ContainedObject {
  public:
    /// typedef for std::vector of HltObjectSummary
    typedef std::vector<HltObjectSummary*>       Vector;
    typedef std::vector<const HltObjectSummary*> ConstVector;

    /// typedef for ObjectVector of HltObjectSummary
    typedef ObjectVector<HltObjectSummary> Container;

    /// Numerical info from the object keyed with a string info ID
    typedef GaudiUtils::VectorMap<std::string, float> Info;

    /// Default Constructor
    HltObjectSummary() : m_lhcbIDs(), m_numericalInfo(), m_summarizedObjectCLID( 0 ), m_summarizedObject( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// numericalInfo from the summarized objected including the parts stored on subobjects. Users should use this
    /// method instead of locally stored numericalInfo.
    HltObjectSummary::Info numericalInfoFlattened() const;

    /// lhcbIDs from the summarized object including the parts stored on subobjects. Users should use this method
    /// instead of locally stored lhcbIDs.
    std::vector<LHCb::LHCbID> lhcbIDsFlattened() const;

    /// substructure of the summarized object (e.g. particle daughters, track of particle with no daughters etc.) -
    /// returns substructure of the bottom subobject
    SmartRefVector<LHCb::HltObjectSummary> substructureFlattened() const;

    /// Add new information, associated with the specified key. This method cannot be used to modify information for an
    /// already existing key
    bool addToInfo( const std::string& key, const float info );

    /// string keys present in numericalInfo
    std::vector<std::string> numericalInfoKeys() const;

    /// string keys present in numericalInfoFlattened
    std::vector<std::string> numericalInfoFlattenedKeys() const;

    /// intelligent printout
    std::ostream& fillStream( std::ostream& ss ) const override;

    /// Retrieve const  List of of LHCbIDs stored directly here. Not expected for obj summary with a substructure. See
    /// also Flattened access method.
    const std::vector<LHCb::LHCbID>& lhcbIDs() const;

    /// Update  List of of LHCbIDs stored directly here. Not expected for obj summary with a substructure. See also
    /// Flattened access method.
    void setLhcbIDs( const std::vector<LHCb::LHCbID>& value );

    /// Retrieve const  List of numerical information from the object stored directly here. See  also Flattened access
    /// method.
    const Info& numericalInfo() const;

    /// Update  List of numerical information from the object stored directly here. See  also Flattened access method.
    void setNumericalInfo( const Info& value );

    /// Retrieve const  Class ID of the object being summarized.
    unsigned int summarizedObjectCLID() const;

    /// Update  Class ID of the object being summarized.
    void setSummarizedObjectCLID( unsigned int value );

    /// Retrieve const  Pointer to the object being summarized. Non persistent.
    const ContainedObject* summarizedObject() const;

    /// Update  Pointer to the object being summarized. Non persistent.
    void setSummarizedObject( const ContainedObject* value );

    /// Retrieve (const)  Either pointer to subobject of the same type or pointers to objects on which this one was
    /// built ( e.g. particle daughters, tracks making the vertex etc.). See also Flattened access method.
    const SmartRefVector<LHCb::HltObjectSummary>& substructure() const;

    /// Update  Either pointer to subobject of the same type or pointers to objects on which this one was built ( e.g.
    /// particle daughters, tracks making the vertex etc.). See also Flattened access method.
    void setSubstructure( const SmartRefVector<LHCb::HltObjectSummary>& value );

    /// Add to  Either pointer to subobject of the same type or pointers to objects on which this one was built ( e.g.
    /// particle daughters, tracks making the vertex etc.). See also Flattened access method.
    void addToSubstructure( const SmartRef<LHCb::HltObjectSummary>& value );

    /// Att to (pointer)  Either pointer to subobject of the same type or pointers to objects on which this one was
    /// built ( e.g. particle daughters, tracks making the vertex etc.). See also Flattened access method.
    void addToSubstructure( const LHCb::HltObjectSummary* value );

    /// Remove from  Either pointer to subobject of the same type or pointers to objects on which this one was built (
    /// e.g. particle daughters, tracks making the vertex etc.). See also Flattened access method.
    void removeFromSubstructure( const SmartRef<LHCb::HltObjectSummary>& value );

    /// Clear  Either pointer to subobject of the same type or pointers to objects on which this one was built ( e.g.
    /// particle daughters, tracks making the vertex etc.). See also Flattened access method.
    void clearSubstructure();

    /// Retrieve (const)  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    const SmartRefVector<LHCb::HltObjectSummary>& substructureExtended() const;

    /// Update  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    void setSubstructureExtended( const SmartRefVector<LHCb::HltObjectSummary>& value );

    /// Add to  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    void addToSubstructureExtended( const SmartRef<LHCb::HltObjectSummary>& value );

    /// Att to (pointer)  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    void addToSubstructureExtended( const LHCb::HltObjectSummary* value );

    /// Remove from  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    void removeFromSubstructureExtended( const SmartRef<LHCb::HltObjectSummary>& value );

    /// Clear  substructure (used in TisTos) plus any extended substructure added for Turbo stream.
    void clearSubstructureExtended();

    friend std::ostream& operator<<( std::ostream& str, const HltObjectSummary& obj ) { return obj.fillStream( str ); }

  protected:
    std::vector<LHCb::LHCbID> m_lhcbIDs; ///< List of of LHCbIDs stored directly here. Not expected for obj summary with
                                         ///< a substructure. See also Flattened access method.
    Info m_numericalInfo; ///< List of numerical information from the object stored directly here. See  also Flattened
                          ///< access method.
    unsigned int           m_summarizedObjectCLID;         ///< Class ID of the object being summarized.
    const ContainedObject* m_summarizedObject;             ///< Pointer to the object being summarized. Non persistent.
    SmartRefVector<LHCb::HltObjectSummary> m_substructure; ///< Either pointer to subobject of the same type or pointers
                                                           ///< to objects on which this one was built ( e.g. particle
                                                           ///< daughters, tracks making the vertex etc.). See also
                                                           ///< Flattened access method.
    SmartRefVector<LHCb::HltObjectSummary> m_substructureExtended; ///< substructure (used in TisTos) plus any extended
                                                                   ///< substructure added for Turbo stream.

  private:
  }; // class HltObjectSummary

  /// Definition of vector container type for HltObjectSummary
  typedef ObjectVector<HltObjectSummary> HltObjectSummarys;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltObjectSummary::clID() const { return LHCb::HltObjectSummary::classID(); }

inline const CLID& LHCb::HltObjectSummary::classID() { return CLID_HltObjectSummary; }

inline const std::vector<LHCb::LHCbID>& LHCb::HltObjectSummary::lhcbIDs() const { return m_lhcbIDs; }

inline void LHCb::HltObjectSummary::setLhcbIDs( const std::vector<LHCb::LHCbID>& value ) { m_lhcbIDs = value; }

inline const LHCb::HltObjectSummary::Info& LHCb::HltObjectSummary::numericalInfo() const { return m_numericalInfo; }

inline void LHCb::HltObjectSummary::setNumericalInfo( const Info& value ) { m_numericalInfo = value; }

inline unsigned int LHCb::HltObjectSummary::summarizedObjectCLID() const { return m_summarizedObjectCLID; }

inline void LHCb::HltObjectSummary::setSummarizedObjectCLID( unsigned int value ) { m_summarizedObjectCLID = value; }

inline const ContainedObject* LHCb::HltObjectSummary::summarizedObject() const { return m_summarizedObject; }

inline void LHCb::HltObjectSummary::setSummarizedObject( const ContainedObject* value ) { m_summarizedObject = value; }

inline const SmartRefVector<LHCb::HltObjectSummary>& LHCb::HltObjectSummary::substructure() const {
  return m_substructure;
}

inline void LHCb::HltObjectSummary::setSubstructure( const SmartRefVector<LHCb::HltObjectSummary>& value ) {
  m_substructure = value;
}

inline void LHCb::HltObjectSummary::addToSubstructure( const SmartRef<LHCb::HltObjectSummary>& value ) {
  m_substructure.push_back( value );
}

inline void LHCb::HltObjectSummary::addToSubstructure( const LHCb::HltObjectSummary* value ) {
  m_substructure.push_back( value );
}

inline void LHCb::HltObjectSummary::removeFromSubstructure( const SmartRef<LHCb::HltObjectSummary>& value ) {
  auto i = std::remove( m_substructure.begin(), m_substructure.end(), value );
  m_substructure.erase( i, m_substructure.end() );
}

inline void LHCb::HltObjectSummary::clearSubstructure() { m_substructure.clear(); }

inline const SmartRefVector<LHCb::HltObjectSummary>& LHCb::HltObjectSummary::substructureExtended() const {
  return m_substructureExtended;
}

inline void LHCb::HltObjectSummary::setSubstructureExtended( const SmartRefVector<LHCb::HltObjectSummary>& value ) {
  m_substructureExtended = value;
}

inline void LHCb::HltObjectSummary::addToSubstructureExtended( const SmartRef<LHCb::HltObjectSummary>& value ) {
  m_substructureExtended.push_back( value );
}

inline void LHCb::HltObjectSummary::addToSubstructureExtended( const LHCb::HltObjectSummary* value ) {
  m_substructureExtended.push_back( value );
}

inline void LHCb::HltObjectSummary::removeFromSubstructureExtended( const SmartRef<LHCb::HltObjectSummary>& value ) {
  auto i = std::remove( m_substructureExtended.begin(), m_substructureExtended.end(), value );
  m_substructureExtended.erase( i, m_substructureExtended.end() );
}

inline void LHCb::HltObjectSummary::clearSubstructureExtended() { m_substructureExtended.clear(); }

inline LHCb::HltObjectSummary::Info LHCb::HltObjectSummary::numericalInfoFlattened() const {

  Info flattenedInfo = m_numericalInfo;
  if ( m_substructure.size() == 1 ) {
    const HltObjectSummary& inner = *( m_substructure[0] );
    if ( inner.summarizedObjectCLID() == m_summarizedObjectCLID ) {
      Info innerInfo = inner.numericalInfoFlattened();
      flattenedInfo.insert( innerInfo.begin(), innerInfo.end() );
    }
  }
  return flattenedInfo;
}

inline std::vector<LHCb::LHCbID> LHCb::HltObjectSummary::lhcbIDsFlattened() const {

  std::vector<LHCb::LHCbID> flattenedLhcbIDs = m_lhcbIDs;
  if ( m_substructure.size() == 1 ) {
    const HltObjectSummary& inner = *( m_substructure[0] );
    if ( inner.summarizedObjectCLID() == m_summarizedObjectCLID ) {
      const auto& innerHits = inner.lhcbIDsFlattened();
      flattenedLhcbIDs.insert( flattenedLhcbIDs.end(), innerHits.begin(), innerHits.end() );
    }
  }
  return flattenedLhcbIDs;
}

inline SmartRefVector<LHCb::HltObjectSummary> LHCb::HltObjectSummary::substructureFlattened() const {

  SmartRefVector<LHCb::HltObjectSummary> subFlattened;
  if ( m_substructure.size() == 1 ) {
    const HltObjectSummary& inner = *( m_substructure[0] );
    if ( inner.summarizedObjectCLID() == m_summarizedObjectCLID ) {
      SmartRefVector<LHCb::HltObjectSummary> innerSub = inner.substructureFlattened();
      subFlattened.insert( subFlattened.end(), innerSub.begin(), innerSub.end() );
    } else {
      subFlattened.insert( subFlattened.end(), m_substructure.begin(), m_substructure.end() );
    }
  } else {
    subFlattened.insert( subFlattened.end(), m_substructure.begin(), m_substructure.end() );
  }
  return subFlattened;
}

inline bool LHCb::HltObjectSummary::addToInfo( const std::string& key, const float info ) {

  return m_numericalInfo.insert( key, info ).second;
}

inline std::vector<std::string> LHCb::HltObjectSummary::numericalInfoKeys() const {

  std::vector<std::string> names;
  names.reserve( m_numericalInfo.size() );
  std::transform( m_numericalInfo.begin(), m_numericalInfo.end(), std::back_inserter( names ),
                  []( const auto& p ) { return p.first; } );
  return names;
}

inline std::vector<std::string> LHCb::HltObjectSummary::numericalInfoFlattenedKeys() const {

  std::vector<std::string> names;
  Info                     nif = numericalInfoFlattened();
  Info::const_iterator     it  = nif.begin();
  while ( it != nif.end() ) names.push_back( ( it++ )->first );
  return names;
}
