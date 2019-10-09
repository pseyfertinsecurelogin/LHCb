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
#include "GaudiKernel/VectorMap.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_LumiFSR = 13501;

  // Namespace for locations in TDS
  namespace LumiFSRLocation {
    inline const std::string Default = "/FileRecords/LumiFSR";
  }

  /** @class LumiFSR LumiFSR.h
   *
   * Accounting class for Lumi in FSR
   *
   * @author Jaap Panman
   *
   */

  class LumiFSR final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of LumiFSR
    typedef std::vector<LumiFSR*>       Vector;
    typedef std::vector<const LumiFSR*> ConstVector;

    /// typedef for KeyedContainer of LumiFSR
    typedef KeyedContainer<LumiFSR, Containers::HashMap> Container;

    /// For User information
    typedef std::pair<int, long long> ValuePair;
    /// User information
    typedef GaudiUtils::VectorMap<int, ValuePair> ExtraInfo;
    /// Set of runNumbers
    typedef std::vector<unsigned int> RunNumbers;
    /// Set of file IDs
    typedef std::vector<std::string> FileIDs;

    /// Default Constructor
    LumiFSR() : m_runNumbers(), m_fileIDs(), m_extraInfo() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Assignment operator
    LumiFSR& operator=( const LHCb::LumiFSR& rhs );

    /// get method
    RunNumbers& runNumbers();

    /// add a RunNumber to the set
    void addRunNumber( unsigned int number );

    /// has information for specified runNumber
    bool hasRunNumber( unsigned int number ) const;

    /// Addition operator
    RunNumbers mergeRuns( const RunNumbers& rhs );

    /// get method
    FileIDs& fileIDs();

    /// add a fileID to the set
    void addFileID( std::string& idString );

    /// has information for specified fileID
    bool hasFileID( const std::string& idString ) const;

    /// Addition operator
    FileIDs mergeFileIDs( const FileIDs& rhs );

    /// ExtraInformation. Don't use directly, use hasInfo, info, addInfo...
    const ExtraInfo extraInfo() const;

    /// has information for specified key
    bool hasInfo( int key ) const;

    ///  Add new information associated with the specified key. This method cannot be used to modify information for a
    ///  pre-existing key.
    bool addInfo( const int key, const int incr, const long long count );

    /// extract the information associated with the given key. If there is no such information the default value will be
    /// returned.
    ValuePair info( const int key, const ValuePair& def ) const;

    /// erase the information associated with the given key
    int eraseInfo( int key );

    /// sums the values for existing keys and inserts a new key if needed
    ExtraInfo mergeInfo( const ExtraInfo& rhs );

    /// increments the values for existing keys and inserts a new key if needed
    ExtraInfo incrementInfo( const int key, const long long count );

    /// Addition operator
    LumiFSR& operator+( const LHCb::LumiFSR& rhs );

    /// Addition operator
    LumiFSR& operator+( const LHCb::LumiFSR* rhs );

    /// Addition operator
    LumiFSR& operator+=( const LHCb::LumiFSR& rhs );

    /// Addition operator
    LumiFSR& operator+=( const LHCb::LumiFSR* rhs );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Set of run numbers summed up in this job
    const RunNumbers& runNumbers() const;

    /// Update  Set of run numbers summed up in this job
    void setRunNumbers( const RunNumbers& value );

    /// Retrieve const  Set of file IDs summed up in this job
    const FileIDs& fileIDs() const;

    /// Update  Set of file IDs summed up in this job
    void setFileIDs( const FileIDs& value );

    /// Update  Some additional user information. Don't use directly. Use *Info() methods.
    void setExtraInfo( const ExtraInfo& value );

    friend std::ostream& operator<<( std::ostream& str, const LumiFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    RunNumbers m_runNumbers; ///< Set of run numbers summed up in this job
    FileIDs    m_fileIDs;    ///< Set of file IDs summed up in this job
    ExtraInfo  m_extraInfo;  ///< Some additional user information. Don't use directly. Use *Info() methods.

  }; // class LumiFSR

  /// Definition of Keyed Container for LumiFSR
  typedef KeyedContainer<LumiFSR, Containers::HashMap> LumiFSRs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::LumiFSR::clID() const { return LHCb::LumiFSR::classID(); }

inline const CLID& LHCb::LumiFSR::classID() { return CLID_LumiFSR; }

inline const LHCb::LumiFSR::RunNumbers& LHCb::LumiFSR::runNumbers() const { return m_runNumbers; }

inline void LHCb::LumiFSR::setRunNumbers( const RunNumbers& value ) { m_runNumbers = value; }

inline const LHCb::LumiFSR::FileIDs& LHCb::LumiFSR::fileIDs() const { return m_fileIDs; }

inline void LHCb::LumiFSR::setFileIDs( const FileIDs& value ) { m_fileIDs = value; }

inline void LHCb::LumiFSR::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

inline LHCb::LumiFSR& LHCb::LumiFSR::operator=( const LHCb::LumiFSR& rhs ) {

  if ( this != &rhs ) {

    mergeRuns( rhs.m_runNumbers );
    mergeFileIDs( rhs.m_fileIDs );
    mergeInfo( rhs.m_extraInfo );
  }
  return *this;
}

inline LHCb::LumiFSR::RunNumbers& LHCb::LumiFSR::runNumbers() { return m_runNumbers; }

inline void LHCb::LumiFSR::addRunNumber( unsigned int number ) { m_runNumbers.push_back( number ); }

inline bool LHCb::LumiFSR::hasRunNumber( unsigned int number ) const {

  return m_runNumbers.end() != find( m_runNumbers.begin(), m_runNumbers.end(), number );
}

inline LHCb::LumiFSR::RunNumbers LHCb::LumiFSR::mergeRuns( const RunNumbers& rhs ) {

  // concatenate run number set (cannot use "set" - have to do it by hand!)
  RunNumbers::const_iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    if ( !hasRunNumber( *( iterB ) ) ) { m_runNumbers.push_back( *( iterB ) ); }
  }

  return m_runNumbers;
}

inline LHCb::LumiFSR::FileIDs& LHCb::LumiFSR::fileIDs() { return m_fileIDs; }

inline void LHCb::LumiFSR::addFileID( std::string& idString ) { m_fileIDs.push_back( idString ); }

inline bool LHCb::LumiFSR::hasFileID( const std::string& idString ) const {

  return m_fileIDs.end() != find( m_fileIDs.begin(), m_fileIDs.end(), idString );
}

inline LHCb::LumiFSR::FileIDs LHCb::LumiFSR::mergeFileIDs( const FileIDs& rhs ) {

  // concatenate fileID set (cannot use "set" - have to do it by hand!)
  FileIDs::const_iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    if ( !hasFileID( *( iterB ) ) ) { m_fileIDs.push_back( *( iterB ) ); }
  }
  return m_fileIDs;
}

inline const LHCb::LumiFSR::ExtraInfo LHCb::LumiFSR::extraInfo() const { return m_extraInfo; }

inline bool LHCb::LumiFSR::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::LumiFSR::addInfo( const int key, const int incr, const long long count ) {

  ValuePair info( incr, count );
  return m_extraInfo.insert( key, info ).second;
}

inline LHCb::LumiFSR::ValuePair LHCb::LumiFSR::info( const int key, const ValuePair& def ) const {

  ExtraInfo::iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline int LHCb::LumiFSR::eraseInfo( int key ) { return m_extraInfo.erase( key ); }

inline LHCb::LumiFSR::ExtraInfo LHCb::LumiFSR::mergeInfo( const ExtraInfo& rhs ) {

  // sum info or add new key
  ExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int       key    = ( *iterB ).first;
    ValuePair valueB = ( *iterB ).second;
    ValuePair valueA = info( key, ValuePair( 0, 0 ) );
    if ( hasInfo( key ) ) { eraseInfo( key ); }
    addInfo( key, valueA.first + valueB.first, valueA.second + valueB.second );
  }
  return m_extraInfo;
}

inline LHCb::LumiFSR::ExtraInfo LHCb::LumiFSR::incrementInfo( const int key, const long long count ) {

  // increment info or add new key, only if data exists
  if ( count != -1 ) {
    ValuePair value = info( key, ValuePair( 0, 0 ) );
    if ( hasInfo( key ) ) { eraseInfo( key ); }
    addInfo( key, ++value.first, value.second + count );
  }
  return m_extraInfo;
}

inline LHCb::LumiFSR& LHCb::LumiFSR::operator+( const LHCb::LumiFSR& rhs ) {

  if ( this != &rhs ) {
    // merge run number sets
    mergeRuns( rhs.m_runNumbers );
    // merge file ID sets
    mergeFileIDs( rhs.m_fileIDs );
    // merge the extraInfo
    mergeInfo( rhs.m_extraInfo );
  }
  return *this;
}

inline LHCb::LumiFSR& LHCb::LumiFSR::operator+( const LHCb::LumiFSR* rhs ) {

  if ( this != rhs ) {
    // merge run number sets
    mergeRuns( rhs->m_runNumbers );
    // merge file ID sets
    mergeFileIDs( rhs->m_fileIDs );
    // merge the extraInfo
    mergeInfo( rhs->m_extraInfo );
  }
  return *this;
}

inline LHCb::LumiFSR& LHCb::LumiFSR::operator+=( const LHCb::LumiFSR& rhs ) { return *this + rhs; }

inline LHCb::LumiFSR& LHCb::LumiFSR::operator+=( const LHCb::LumiFSR* rhs ) { return *this + *rhs; }

inline std::ostream& LHCb::LumiFSR::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ "
     << " runs : ";

  RunNumbers::const_iterator run;
  for ( run = m_runNumbers.begin(); run != m_runNumbers.end(); run++ ) { ss << *run << " "; }
  LumiFSR::FileIDs::const_iterator file;
  ss << " files : ";
  for ( file = m_fileIDs.begin(); file != m_fileIDs.end(); file++ ) { ss << *file << " "; }

  // overall sum info
  ss << " info (key/incr/integral) : ";
  ExtraInfo::const_iterator infoIter;
  for ( infoIter = m_extraInfo.begin(); infoIter != m_extraInfo.end(); infoIter++ ) {
    // get the key and value of the input info
    int                key    = infoIter->first;
    LumiFSR::ValuePair values = infoIter->second;
    int                incr   = values.first;
    long long          count  = values.second;
    ss << key << " " << incr << " " << count << " / ";
  }
  ss << " }";

  return ss;
}
