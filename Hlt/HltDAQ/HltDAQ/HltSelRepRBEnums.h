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
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltSelRepRBEnums HltSelRepRBEnums.h
   *
   * Enums used in RawBank::HltSelReports sub-bank structure
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelRepRBEnums {
  public:
    /// Sub-bank IDs
    enum SubBankIDs {
      kHitsID      = 0,
      kObjTypID    = 1,
      kSubstrID    = 2,
      kExtraInfoID = 3,
      kStdInfoID   = 4,
      kMaxBankID   = 7,
      kUnknownID   = 8
    };
    /// to report bank integrity
    enum IntegrityCodes {
      kBankIsOK          = 0,
      kEmptyBank         = 1,
      kDuplicateBankIDs  = 2,
      kPtrOutOfSequence  = 3,
      kPtrOutOfRange     = 4,
      kZeroAllocatedSize = 5,
      kNoBank            = 6,
      kUnknownError      = 100
    };

    /// Default Constructor
    HltSelRepRBEnums() {}

    /// Default Destructor
    virtual ~HltSelRepRBEnums() {}

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// conversion of string to enum for type SubBankIDs
    static LHCb::HltSelRepRBEnums::SubBankIDs SubBankIDsToType( const std::string& aName );

    /// conversion to string for enum type SubBankIDs
    static const std::string& SubBankIDsToString( int aEnum );
    /// conversion of string to enum for type IntegrityCodes
    static LHCb::HltSelRepRBEnums::IntegrityCodes IntegrityCodesToType( const std::string& aName );

    /// conversion to string for enum type IntegrityCodes
    static const std::string& IntegrityCodesToString( int aEnum );

    friend std::ostream& operator<<( std::ostream& str, const HltSelRepRBEnums& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    static const GaudiUtils::VectorMap<std::string, SubBankIDs>&     s_SubBankIDsTypMap();
    static const GaudiUtils::VectorMap<std::string, IntegrityCodes>& s_IntegrityCodesTypMap();

  }; // class HltSelRepRBEnums

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBEnums::SubBankIDs e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBEnums::kHitsID:
      return s << "kHitsID";
    case LHCb::HltSelRepRBEnums::kObjTypID:
      return s << "kObjTypID";
    case LHCb::HltSelRepRBEnums::kSubstrID:
      return s << "kSubstrID";
    case LHCb::HltSelRepRBEnums::kExtraInfoID:
      return s << "kExtraInfoID";
    case LHCb::HltSelRepRBEnums::kStdInfoID:
      return s << "kStdInfoID";
    case LHCb::HltSelRepRBEnums::kMaxBankID:
      return s << "kMaxBankID";
    case LHCb::HltSelRepRBEnums::kUnknownID:
      return s << "kUnknownID";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBEnums::SubBankIDs";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::HltSelRepRBEnums::IntegrityCodes e ) {
    switch ( e ) {
    case LHCb::HltSelRepRBEnums::kBankIsOK:
      return s << "kBankIsOK";
    case LHCb::HltSelRepRBEnums::kEmptyBank:
      return s << "kEmptyBank";
    case LHCb::HltSelRepRBEnums::kDuplicateBankIDs:
      return s << "kDuplicateBankIDs";
    case LHCb::HltSelRepRBEnums::kPtrOutOfSequence:
      return s << "kPtrOutOfSequence";
    case LHCb::HltSelRepRBEnums::kPtrOutOfRange:
      return s << "kPtrOutOfRange";
    case LHCb::HltSelRepRBEnums::kZeroAllocatedSize:
      return s << "kZeroAllocatedSize";
    case LHCb::HltSelRepRBEnums::kNoBank:
      return s << "kNoBank";
    case LHCb::HltSelRepRBEnums::kUnknownError:
      return s << "kUnknownError";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HltSelRepRBEnums::IntegrityCodes";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::HltSelRepRBEnums::fillStream( std::ostream& s ) const { return s; }

inline const GaudiUtils::VectorMap<std::string, LHCb::HltSelRepRBEnums::SubBankIDs>&
LHCb::HltSelRepRBEnums::s_SubBankIDsTypMap() {
  static const GaudiUtils::VectorMap<std::string, SubBankIDs> m = {
      {"kHitsID", kHitsID},       {"kObjTypID", kObjTypID},   {"kSubstrID", kSubstrID},  {"kExtraInfoID", kExtraInfoID},
      {"kStdInfoID", kStdInfoID}, {"kMaxBankID", kMaxBankID}, {"kUnknownID", kUnknownID}};
  return m;
}

inline LHCb::HltSelRepRBEnums::SubBankIDs LHCb::HltSelRepRBEnums::SubBankIDsToType( const std::string& aName ) {
  auto iter = s_SubBankIDsTypMap().find( aName );
  return iter != s_SubBankIDsTypMap().end() ? iter->second : kUnknownID;
}

inline const std::string& LHCb::HltSelRepRBEnums::SubBankIDsToString( int aEnum ) {
  static const std::string s_kUnknownID = "kUnknownID";
  auto                     iter         = std::find_if( s_SubBankIDsTypMap().begin(), s_SubBankIDsTypMap().end(),
                            [&]( const std::pair<const std::string, SubBankIDs>& i ) { return i.second == aEnum; } );
  return iter != s_SubBankIDsTypMap().end() ? iter->first : s_kUnknownID;
}
inline const GaudiUtils::VectorMap<std::string, LHCb::HltSelRepRBEnums::IntegrityCodes>&
LHCb::HltSelRepRBEnums::s_IntegrityCodesTypMap() {
  static const GaudiUtils::VectorMap<std::string, IntegrityCodes> m = {{"kBankIsOK", kBankIsOK},
                                                                       {"kEmptyBank", kEmptyBank},
                                                                       {"kDuplicateBankIDs", kDuplicateBankIDs},
                                                                       {"kPtrOutOfSequence", kPtrOutOfSequence},
                                                                       {"kPtrOutOfRange", kPtrOutOfRange},
                                                                       {"kZeroAllocatedSize", kZeroAllocatedSize},
                                                                       {"kNoBank", kNoBank},
                                                                       {"kUnknownError", kUnknownError}};
  return m;
}

inline LHCb::HltSelRepRBEnums::IntegrityCodes LHCb::HltSelRepRBEnums::IntegrityCodesToType( const std::string& aName ) {
  auto iter = s_IntegrityCodesTypMap().find( aName );
  return iter != s_IntegrityCodesTypMap().end() ? iter->second : kUnknownError;
}

inline const std::string& LHCb::HltSelRepRBEnums::IntegrityCodesToString( int aEnum ) {
  static const std::string s_kUnknownError = "kUnknownError";
  auto                     iter =
      std::find_if( s_IntegrityCodesTypMap().begin(), s_IntegrityCodesTypMap().end(),
                    [&]( const std::pair<const std::string, IntegrityCodes>& i ) { return i.second == aEnum; } );
  return iter != s_IntegrityCodesTypMap().end() ? iter->first : s_kUnknownError;
}
