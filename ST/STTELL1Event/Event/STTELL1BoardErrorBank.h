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
#include "Event/STTELL1Error.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_STTELL1BoardErrorBank = 9013;

  // Namespace for locations in TDS
  namespace STTELL1BoardErrorBankLocation {
    inline const std::string TTErrors = "Raw/TT/Errors";
    inline const std::string ITErrors = "Raw/IT/Errors";
  } // namespace STTELL1BoardErrorBankLocation

  /** @class STTELL1BoardErrorBank STTELL1BoardErrorBank.h
   *
   * Error Bank from a Tell1 board
   *
   * @author Mathias Knecht
   *
   */

  class STTELL1BoardErrorBank : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of STTELL1BoardErrorBank
    typedef KeyedContainer<STTELL1BoardErrorBank, Containers::HashMap> Container;

    /// Vector of STTELL1Error typedef
    typedef std::vector<LHCb::STTELL1Error*> ErrorVector;

    /// Default Constructor
    STTELL1BoardErrorBank() : m_errorInfo() {}

    /// destructor
    virtual ~STTELL1BoardErrorBank();

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// add to error vector
    void addToErrorInfo( STTELL1Error* value );

    /// Returns the Tell1D id
    unsigned int Tell1ID() const;

    /// get the error info for a given pp
    STTELL1Error* ppErrorInfo( const unsigned int& pp ) const;

    /// fraction of links that are OK
    double fractionOK( const unsigned int pcn ) const;

    /// true if an TT error bank
    bool isTT() const;

    /// true if an IT error bank
    bool isIT() const;

    /// Print out of error information
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Vector of 4 Error Info (1 per PP)
    const std::vector<LHCb::STTELL1Error*>& errorInfo() const;

    /// Retrieve  Vector of 4 Error Info (1 per PP)
    std::vector<LHCb::STTELL1Error*>& errorInfo();

  protected:
  private:
    /// short cut to the registry name
    std::string regName() const;

    std::vector<LHCb::STTELL1Error*> m_errorInfo; ///< Vector of 4 Error Info (1 per PP)

  }; // class STTELL1BoardErrorBank

  /// Definition of Keyed Container for STTELL1BoardErrorBank
  typedef KeyedContainer<STTELL1BoardErrorBank, Containers::HashMap> STTELL1BoardErrorBanks;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::STTELL1BoardErrorBank::~STTELL1BoardErrorBank() {

  for ( auto& eInfo : m_errorInfo ) delete eInfo;
  m_errorInfo.clear();
}

inline const CLID& LHCb::STTELL1BoardErrorBank::clID() const { return LHCb::STTELL1BoardErrorBank::classID(); }

inline const CLID& LHCb::STTELL1BoardErrorBank::classID() { return CLID_STTELL1BoardErrorBank; }

inline const std::vector<LHCb::STTELL1Error*>& LHCb::STTELL1BoardErrorBank::errorInfo() const { return m_errorInfo; }

inline std::vector<LHCb::STTELL1Error*>& LHCb::STTELL1BoardErrorBank::errorInfo() { return m_errorInfo; }

inline void LHCb::STTELL1BoardErrorBank::addToErrorInfo( STTELL1Error* value ) { m_errorInfo.push_back( value ); }

inline unsigned int LHCb::STTELL1BoardErrorBank::Tell1ID() const { return key(); }

inline LHCb::STTELL1Error* LHCb::STTELL1BoardErrorBank::ppErrorInfo( const unsigned int& pp ) const {

  return ( pp < m_errorInfo.size() ? m_errorInfo[pp] : 0 );
}
