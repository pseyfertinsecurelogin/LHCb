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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "Kernel/CondDBNameTagPair.h"
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_ProcessFSR = 13507;

  // Namespace for locations in TDS
  namespace ProcessFSRLocation {
    inline const std::string Default = "/FileRecords/ProcessFSR";
  }

  /** @class ProcessFSR ProcessFSR.h
   *
   * FSR based upon common base class for all process headers: GenHeader,
   * MCHeader...
   *
   * @author R. Lambert
   *
   */

  class ProcessFSR : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of ProcessFSR
    typedef KeyedContainer<ProcessFSR, Containers::HashMap> Container;

    /// Default Constructor
    ProcessFSR() : m_applicationName( "Unknown" ), m_applicationVersion( "Unknown" ), m_condDBTags(), m_FID() {}

    /// Default Destructor
    virtual ~ProcessFSR() {}

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

    /// Retrieve const  Conditions database tags
    const std::vector<LHCb::CondDBNameTagPair>& condDBTags() const;

    /// Update  Conditions database tags
    void setCondDBTags( const std::vector<LHCb::CondDBNameTagPair>& value );

    /// Retrieve const  Only used after merging, should be filled with file GUID
    const std::string& fid() const;

    /// Update  Only used after merging, should be filled with file GUID
    void setFID( const std::string& value );

    friend std::ostream& operator<<( std::ostream& str, const ProcessFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::string m_applicationName;                     ///< Application or Program Name (e.g. Gauss, Boole or Pythia)
    std::string m_applicationVersion;                  ///< Application or Program version
    std::vector<LHCb::CondDBNameTagPair> m_condDBTags; ///< Conditions database tags
    std::string                          m_FID;        ///< Only used after merging, should be filled with file GUID

  }; // class ProcessFSR

  /// Definition of Keyed Container for ProcessFSR
  typedef KeyedContainer<ProcessFSR, Containers::HashMap> ProcessFSRs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::ProcessFSR::clID() const { return LHCb::ProcessFSR::classID(); }

inline const CLID& LHCb::ProcessFSR::classID() { return CLID_ProcessFSR; }

inline std::ostream& LHCb::ProcessFSR::fillStream( std::ostream& s ) const {
  s << "{ "
    << "applicationName :	" << m_applicationName << std::endl
    << "applicationVersion :	" << m_applicationVersion << std::endl
    << "condDBTags :	" << m_condDBTags << std::endl
    << "FID :	" << m_FID << std::endl
    << " }";
  return s;
}

inline const std::string& LHCb::ProcessFSR::applicationName() const { return m_applicationName; }

inline void LHCb::ProcessFSR::setApplicationName( const std::string& value ) { m_applicationName = value; }

inline const std::string& LHCb::ProcessFSR::applicationVersion() const { return m_applicationVersion; }

inline void LHCb::ProcessFSR::setApplicationVersion( const std::string& value ) { m_applicationVersion = value; }

inline const std::vector<LHCb::CondDBNameTagPair>& LHCb::ProcessFSR::condDBTags() const { return m_condDBTags; }

inline void LHCb::ProcessFSR::setCondDBTags( const std::vector<LHCb::CondDBNameTagPair>& value ) {
  m_condDBTags = value;
}

inline const std::string& LHCb::ProcessFSR::fid() const { return m_FID; }

inline void LHCb::ProcessFSR::setFID( const std::string& value ) { m_FID = value; }
