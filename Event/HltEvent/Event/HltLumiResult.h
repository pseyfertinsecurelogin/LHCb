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
  static const CLID CLID_HltLumiResult = 7515;

  // Namespace for locations in TDS
  namespace HltLumiResultLocation {
    inline const std::string Default = "Hlt/LumiResult";
  }

  /** @class HltLumiResult HltLumiResult.h
   *
   * Result class for Lumi in Hlt
   *
   * @author Jaap Panman
   *
   */

  class HltLumiResult final : public DataObject {
  public:
    /// User information
    typedef GaudiUtils::VectorMap<int, double> ExtraInfo;

    /// Default Constructor
    HltLumiResult() : m_extraInfo() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Assignment operator
    HltLumiResult& operator=( const HltLumiResult& rhs );

    /// ExtraInformation. Don't use directly, use hasInfo, info, addInfo...
    const ExtraInfo extraInfo() const;

    /// has information for specified key
    bool hasInfo( int key ) const;

    ///  Add new information associated with the specified key. This method cannot be used to modify information for a
    ///  pre-existing key.
    bool addInfo( const int key, const double info );

    /// extract the information associated with the given key. If there is no such infomration the default value will be
    /// returned.
    double info( const int key, const double def ) const;

    /// erase the information associated with the given key
    unsigned long eraseInfo( int key );

    /// Update  Some addtional user information. Don't use directly. Use *Info() methods.
    void setExtraInfo( const ExtraInfo& value );

    friend std::ostream& operator<<( std::ostream& str, const HltLumiResult& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    ExtraInfo m_extraInfo; ///< Some addtional user information. Don't use directly. Use *Info() methods.

  }; // class HltLumiResult

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltLumiResult::clID() const { return LHCb::HltLumiResult::classID(); }

inline const CLID& LHCb::HltLumiResult::classID() { return CLID_HltLumiResult; }

inline std::ostream& LHCb::HltLumiResult::fillStream( std::ostream& s ) const {
  s << "{ "
    << "extraInfo :	" << m_extraInfo << std::endl
    << " }";
  return s;
}

inline void LHCb::HltLumiResult::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

inline LHCb::HltLumiResult& LHCb::HltLumiResult::operator=( const HltLumiResult& rhs ) {

  if ( this != &rhs ) { m_extraInfo = rhs.m_extraInfo; }
  return *this;
}

inline const LHCb::HltLumiResult::ExtraInfo LHCb::HltLumiResult::extraInfo() const { return m_extraInfo; }

inline bool LHCb::HltLumiResult::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::HltLumiResult::addInfo( const int key, const double info ) {

  return m_extraInfo.insert( key, info ).second;
}

inline double LHCb::HltLumiResult::info( const int key, const double def ) const {

  ExtraInfo::iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline unsigned long LHCb::HltLumiResult::eraseInfo( int key ) { return m_extraInfo.erase( key ); }
