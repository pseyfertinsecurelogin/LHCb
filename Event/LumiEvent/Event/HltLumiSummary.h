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
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HltLumiSummary = 7520;

  // Namespace for locations in TDS
  namespace HltLumiSummaryLocation {
    inline const std::string Default = "Hlt/LumiSummary";
  }

  /** @class HltLumiSummary HltLumiSummary.h
   *
   * Summary class for Lumi in Hlt
   *
   * @author Jaap Panman
   *
   */

  class HltLumiSummary final : public DataObject {
  public:
    /// User information
    typedef GaudiUtils::VectorMap<int, int> ExtraInfo;

    /// Default Constructor
    HltLumiSummary() : m_extraInfo() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Clone LumiSummary
    virtual HltLumiSummary* clone() const;

    /// ExtraInformation. Don't use directly, use hasInfo, info, addInfo...
    const ExtraInfo extraInfo() const;

    /// has information for specified key
    bool hasInfo( int key ) const;

    ///  Add new information associated with the specified key. This method cannot be used to modify information for a
    ///  pre-existing key.
    bool addInfo( const int key, const int info );

    /// extract the information associated with the given key. If there is no such infomration the default value will be
    /// returned.
    int info( const int key, const int def ) const;

    /// erase the information associated with the given key
    unsigned long eraseInfo( int key );

    /// Update  Some addtional user information. Don't use directly. Use *Info() methods.
    void setExtraInfo( const ExtraInfo& value );

    friend std::ostream& operator<<( std::ostream& str, const HltLumiSummary& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    ExtraInfo m_extraInfo; ///< Some addtional user information. Don't use directly. Use *Info() methods.

  }; // class HltLumiSummary

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltLumiSummary::clID() const { return LHCb::HltLumiSummary::classID(); }

inline const CLID& LHCb::HltLumiSummary::classID() { return CLID_HltLumiSummary; }

inline std::ostream& LHCb::HltLumiSummary::fillStream( std::ostream& s ) const {
  s << "{ "
    << "extraInfo :	" << m_extraInfo << std::endl
    << " }";
  return s;
}

inline void LHCb::HltLumiSummary::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

inline LHCb::HltLumiSummary* LHCb::HltLumiSummary::clone() const { return new HltLumiSummary( *this ); }

inline const LHCb::HltLumiSummary::ExtraInfo LHCb::HltLumiSummary::extraInfo() const { return m_extraInfo; }

inline bool LHCb::HltLumiSummary::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::HltLumiSummary::addInfo( const int key, const int info ) {

  return m_extraInfo.insert( key, info ).second;
}

inline int LHCb::HltLumiSummary::info( const int key, const int def ) const {

  ExtraInfo::iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline unsigned long LHCb::HltLumiSummary::eraseInfo( int key ) { return m_extraInfo.erase( key ); }
