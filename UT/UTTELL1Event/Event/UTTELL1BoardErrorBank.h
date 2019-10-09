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
#include "Event/UTTELL1Error.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTTELL1BoardErrorBank = 9013;

  // Namespace for locations in TDS
  namespace UTTELL1BoardErrorBankLocation {
    inline const std::string UTErrors = "Raw/UT/Errors";
  }

  /** @class UTTELL1BoardErrorBank UTTELL1BoardErrorBank.h
   *
   * Error Bank from a Tell1 board
   *
   * @author Andy Beiter (based on code by Mathias Knecht)
   *
   */

  class UTTELL1BoardErrorBank : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of UTTELL1BoardErrorBank
    typedef KeyedContainer<UTTELL1BoardErrorBank, Containers::HashMap> Container;

    /// Vector of UTTELL1Error typedef
    typedef std::vector<LHCb::UTTELL1Error*> ErrorVector;

    /// Default Constructor
    UTTELL1BoardErrorBank() : m_errorInfo() {}

    /// destructor
    virtual ~UTTELL1BoardErrorBank();

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// add to error vector
    void addToErrorInfo( UTTELL1Error* value );

    /// Returns the Tell1D id
    unsigned int Tell1ID() const;

    /// get the error info for a given pp
    UTTELL1Error* ppErrorInfo( const unsigned int& pp ) const;

    /// fraction of links that are OK
    double fractionOK( const unsigned int pcn ) const;

    /// true if an UT error bank
    bool isUT() const;

    /// Print out of error information
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Vector of 4 Error Info (1 per PP)
    const std::vector<LHCb::UTTELL1Error*>& errorInfo() const;

    /// Retrieve  Vector of 4 Error Info (1 per PP)
    std::vector<LHCb::UTTELL1Error*>& errorInfo();

  protected:
  private:
    /// short cut to the registry name
    std::string regName() const;

    std::vector<LHCb::UTTELL1Error*> m_errorInfo; ///< Vector of 4 Error Info (1 per PP)

  }; // class UTTELL1BoardErrorBank

  /// Definition of Keyed Container for UTTELL1BoardErrorBank
  typedef KeyedContainer<UTTELL1BoardErrorBank, Containers::HashMap> UTTELL1BoardErrorBanks;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::UTTELL1BoardErrorBank::~UTTELL1BoardErrorBank() {

  for ( auto& eInfo : m_errorInfo ) delete eInfo;
  m_errorInfo.clear();
}

inline const CLID& LHCb::UTTELL1BoardErrorBank::clID() const { return LHCb::UTTELL1BoardErrorBank::classID(); }

inline const CLID& LHCb::UTTELL1BoardErrorBank::classID() { return CLID_UTTELL1BoardErrorBank; }

inline const std::vector<LHCb::UTTELL1Error*>& LHCb::UTTELL1BoardErrorBank::errorInfo() const { return m_errorInfo; }

inline std::vector<LHCb::UTTELL1Error*>& LHCb::UTTELL1BoardErrorBank::errorInfo() { return m_errorInfo; }

inline void LHCb::UTTELL1BoardErrorBank::addToErrorInfo( UTTELL1Error* value ) { m_errorInfo.push_back( value ); }

inline unsigned int LHCb::UTTELL1BoardErrorBank::Tell1ID() const { return key(); }

inline LHCb::UTTELL1Error* LHCb::UTTELL1BoardErrorBank::ppErrorInfo( const unsigned int& pp ) const {

  return ( pp < m_errorInfo.size() ? m_errorInfo[pp] : 0 );
}
