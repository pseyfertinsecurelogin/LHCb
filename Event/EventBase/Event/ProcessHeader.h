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
#include "GaudiKernel/SerializeSTL.h"
#include "Kernel/CondDBNameTagPair.h"
#include "Kernel/PlatformInfo.h"
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_ProcessHeader = 103;

  // Namespace for locations in TDS
  namespace ProcessHeaderLocation {
    inline const std::string Digi = "MC/DigiHeader";
    inline const std::string MC   = "MC/Header";
    inline const std::string Rec  = "Rec/Header";
  } // namespace ProcessHeaderLocation

  /** @class ProcessHeader ProcessHeader.h
   *
   * Common base class for all process headers: GenHeader, MCHeader...
   *
   * @author P. Koppenburg
   *
   */

  class ProcessHeader : public DataObject {
  public:
    /// Copy constructor. Creates a new ProcessHeader with the same information.
    ProcessHeader( const LHCb::ProcessHeader& head )
        : DataObject()
        , m_applicationName( head.applicationName() )
        , m_applicationVersion( head.applicationVersion() )
        , m_runNumber( head.runNumber() )
        , m_condDBTags( head.condDBTags() )
        , m_platformInfo( head.platformInfo() ) {}

    /// Default Constructor
    ProcessHeader()
        : m_applicationName( "Unknown" )
        , m_applicationVersion( "Unknown" )
        , m_runNumber( 0 )
        , m_condDBTags()
        , m_platformInfo() {}

    /// Default Destructor
    virtual ~ProcessHeader() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Application or Program Name (e.g. Gauss, Boole or Pythia)
    const std::string& applicationName() const;

    /// Update  Application or Program Name (e.g. Gauss, Boole or Pythia)
    void setApplicationName( const std::string& value );

    /// Retrieve const  Application or Program version
    const std::string& applicationVersion() const;

    /// Update  Application or Program version
    void setApplicationVersion( const std::string& value );

    /// Retrieve const  Run number
    unsigned int runNumber() const;

    /// Update  Run number
    void setRunNumber( unsigned int value );

    /// Retrieve const  Conditions database tags
    const std::vector<LHCb::CondDBNameTagPair>& condDBTags() const;

    /// Update  Conditions database tags
    void setCondDBTags( const std::vector<LHCb::CondDBNameTagPair>& value );

    /// Retrieve const  Details of binary and host during processing
    const LHCb::PlatformInfo& platformInfo() const;

    /// Update  Details of binary and host during processing
    void setPlatformInfo( const LHCb::PlatformInfo& value );

    friend std::ostream& operator<<( std::ostream& str, const ProcessHeader& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::string  m_applicationName;                      ///< Application or Program Name (e.g. Gauss, Boole or Pythia)
    std::string  m_applicationVersion;                   ///< Application or Program version
    unsigned int m_runNumber;                            ///< Run number
    std::vector<LHCb::CondDBNameTagPair> m_condDBTags;   ///< Conditions database tags
    LHCb::PlatformInfo                   m_platformInfo; ///< Details of binary and host during processing

  }; // class ProcessHeader

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::ProcessHeader::clID() const { return LHCb::ProcessHeader::classID(); }

inline const CLID& LHCb::ProcessHeader::classID() { return CLID_ProcessHeader; }

inline std::ostream& LHCb::ProcessHeader::fillStream( std::ostream& s ) const {
  s << "{ "
    << "applicationName :	" << m_applicationName << std::endl
    << "applicationVersion :	" << m_applicationVersion << std::endl
    << "runNumber :	" << m_runNumber << std::endl
    << "condDBTags :	" << m_condDBTags << std::endl
    << "platformInfo :	" << m_platformInfo << std::endl
    << " }";
  return s;
}

inline const std::string& LHCb::ProcessHeader::applicationName() const { return m_applicationName; }

inline void LHCb::ProcessHeader::setApplicationName( const std::string& value ) { m_applicationName = value; }

inline const std::string& LHCb::ProcessHeader::applicationVersion() const { return m_applicationVersion; }

inline void LHCb::ProcessHeader::setApplicationVersion( const std::string& value ) { m_applicationVersion = value; }

inline unsigned int LHCb::ProcessHeader::runNumber() const { return m_runNumber; }

inline void LHCb::ProcessHeader::setRunNumber( unsigned int value ) { m_runNumber = value; }

inline const std::vector<LHCb::CondDBNameTagPair>& LHCb::ProcessHeader::condDBTags() const { return m_condDBTags; }

inline void LHCb::ProcessHeader::setCondDBTags( const std::vector<LHCb::CondDBNameTagPair>& value ) {
  m_condDBTags = value;
}

inline const LHCb::PlatformInfo& LHCb::ProcessHeader::platformInfo() const { return m_platformInfo; }

inline void LHCb::ProcessHeader::setPlatformInfo( const LHCb::PlatformInfo& value ) { m_platformInfo = value; }
