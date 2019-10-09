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
#include "Event/LumiFSR.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/VectorMap.h"
#include <math.h>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_LumiIntegral = 13502;

  // Namespace for locations in TDS
  namespace LumiIntegralLocation {
    inline const std::string Default = "/FileRecords/EOR/LumiIntegral";
  }

  /** @class LumiIntegral LumiIntegral.h
   *
   * Accounting class for Lumi in FSR
   *
   * @author Jaap Panman
   *
   */

  class LumiIntegral final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of LumiIntegral
    typedef std::vector<LumiIntegral*>       Vector;
    typedef std::vector<const LumiIntegral*> ConstVector;

    /// typedef for KeyedContainer of LumiIntegral
    typedef KeyedContainer<LumiIntegral, Containers::HashMap> Container;

    /// For User information
    typedef std::pair<int, double> ValuePair;
    /// User information
    typedef GaudiUtils::VectorMap<int, ValuePair> ExtraInfo;
    /// For User information
    typedef std::pair<int, long long> LongValuePair;
    /// User information
    typedef GaudiUtils::VectorMap<int, LongValuePair> LongExtraInfo;
    /// Set of runNumbers
    typedef std::vector<unsigned int> RunNumbers;
    /// Set of file IDs
    typedef std::vector<std::string> FileIDs;

    /// Default Constructor
    LumiIntegral() : m_runNumbers(), m_fileIDs(), m_extraInfo() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Assignment operator
    LumiIntegral& operator=( const LumiIntegral& rhs );

    /// add a RunNumber to the set
    void addRunNumber( unsigned int number );

    /// has information for specified runNumber
    bool hasRunNumber( unsigned int number ) const;

    /// Addition operator
    RunNumbers mergeRuns( const RunNumbers& rhs );

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
    bool addInfo( const int key, const int incr, const double count );

    /// extract the information associated with the given key. If there is no such information the default value will be
    /// returned.
    ValuePair info( const int key, const ValuePair& def ) const;

    /// erase the information associated with the given key
    int eraseInfo( int key );

    /// scales the values for existing keys with a vector of double
    ExtraInfo scale( const std::vector<double>& scalefactor );

    /// sums the values for existing keys and inserts a new key if needed
    ExtraInfo mergeInfo( const ExtraInfo& rhs );

    /// sums the values for existing keys and inserts a new key if needed
    ExtraInfo mergeInfo( const LongExtraInfo& rhs );

    /// sums the values for existing keys normalized to *this
    ExtraInfo mergeInfoNormalized( const ExtraInfo& rhs, const double factor );

    /// sums the values for existing keys normalized to *this
    ExtraInfo mergeInfoNormalized( const LongExtraInfo& rhs, const double factor );

    /// sums the values for existing keys normalized to *this
    ExtraInfo mergeInfoNormalized( const ExtraInfo& rhs, const double factor, const int fromkey );

    /// sums the values for existing keys normalized to *this
    ExtraInfo mergeInfoNormalized( const LongExtraInfo& rhs, const double factor, const int fromkey );

    /// increments the values for existing keys and inserts a new key if needed
    ExtraInfo incrementInfo( const int key, const double count );

    /// Addition operator
    LumiIntegral& operator+( const LHCb::LumiIntegral& rhs );

    /// Addition operator
    LumiIntegral& operator+( const LHCb::LumiIntegral* rhs );

    /// Addition (increment) operator
    LumiIntegral& operator+=( const LHCb::LumiIntegral& rhs );

    /// Addition (increment) operator
    LumiIntegral& operator+=( const LHCb::LumiIntegral* rhs );

    /// Addition operator
    LumiIntegral& operator+( const LHCb::LumiFSR& rhs );

    /// Addition operator
    LumiIntegral& operator+( const LHCb::LumiFSR* rhs );

    /// Addition (increment) operator
    LumiIntegral& operator+=( const LHCb::LumiFSR& rhs );

    /// Addition (increment) operator
    LumiIntegral& operator+=( const LHCb::LumiFSR* rhs );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiIntegral& rhs, const double factor );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiIntegral* rhs, const double factor );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiFSR& rhs, const double factor );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiFSR* rhs, const double factor );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiIntegral& rhs, const double factor, const int fromkey );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiIntegral* rhs, const double factor, const int fromkey );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiFSR& rhs, const double factor, const int fromkey );

    /// Add scaled to norm
    LumiIntegral& addNormalized( const LHCb::LumiFSR* rhs, const double factor, const int fromkey );

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

    friend std::ostream& operator<<( std::ostream& str, const LumiIntegral& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    RunNumbers m_runNumbers; ///< Set of run numbers summed up in this job
    FileIDs    m_fileIDs;    ///< Set of file IDs summed up in this job
    ExtraInfo  m_extraInfo;  ///< Some additional user information. Don't use directly. Use *Info() methods.

  }; // class LumiIntegral

  /// Definition of Keyed Container for LumiIntegral
  typedef KeyedContainer<LumiIntegral, Containers::HashMap> LumiIntegrals;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::LumiIntegral::clID() const { return LHCb::LumiIntegral::classID(); }

inline const CLID& LHCb::LumiIntegral::classID() { return CLID_LumiIntegral; }

inline const LHCb::LumiIntegral::RunNumbers& LHCb::LumiIntegral::runNumbers() const { return m_runNumbers; }

inline void LHCb::LumiIntegral::setRunNumbers( const RunNumbers& value ) { m_runNumbers = value; }

inline const LHCb::LumiIntegral::FileIDs& LHCb::LumiIntegral::fileIDs() const { return m_fileIDs; }

inline void LHCb::LumiIntegral::setFileIDs( const FileIDs& value ) { m_fileIDs = value; }

inline void LHCb::LumiIntegral::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator=( const LumiIntegral& rhs ) {

  if ( this != &rhs ) {
    m_runNumbers = rhs.m_runNumbers;
    m_fileIDs    = rhs.m_fileIDs;
    m_extraInfo  = rhs.m_extraInfo;
  }
  return *this;
}

inline void LHCb::LumiIntegral::addRunNumber( unsigned int number ) { m_runNumbers.push_back( number ); }

inline bool LHCb::LumiIntegral::hasRunNumber( unsigned int number ) const {

  return m_runNumbers.end() != find( m_runNumbers.begin(), m_runNumbers.end(), number );
}

inline LHCb::LumiIntegral::RunNumbers LHCb::LumiIntegral::mergeRuns( const RunNumbers& rhs ) {

  // concatenate run number set (cannot use "set" - have to do it by hand!)
  RunNumbers::const_iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    if ( !hasRunNumber( *( iterB ) ) ) { m_runNumbers.push_back( *( iterB ) ); }
  }

  return m_runNumbers;
}

inline void LHCb::LumiIntegral::addFileID( std::string& idString ) { m_fileIDs.push_back( idString ); }

inline bool LHCb::LumiIntegral::hasFileID( const std::string& idString ) const {

  return m_fileIDs.end() != find( m_fileIDs.begin(), m_fileIDs.end(), idString );
}

inline LHCb::LumiIntegral::FileIDs LHCb::LumiIntegral::mergeFileIDs( const FileIDs& rhs ) {

  // concatenate fileID set (cannot use "set" - have to do it by hand!)
  FileIDs::const_iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    if ( !hasFileID( *( iterB ) ) ) { m_fileIDs.push_back( *( iterB ) ); }
  }
  return m_fileIDs;
}

inline const LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::extraInfo() const { return m_extraInfo; }

inline bool LHCb::LumiIntegral::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::LumiIntegral::addInfo( const int key, const int incr, const double count ) {

  ValuePair info( incr, count );
  return m_extraInfo.insert( key, info ).second;
}

inline LHCb::LumiIntegral::ValuePair LHCb::LumiIntegral::info( const int key, const ValuePair& def ) const {

  ExtraInfo::iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline int LHCb::LumiIntegral::eraseInfo( int key ) { return m_extraInfo.erase( key ); }

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::scale( const std::vector<double>& scalefactor ) {

  // scale existing info values
  ExtraInfo::const_iterator infoIter;
  for ( infoIter = m_extraInfo.begin(); infoIter != m_extraInfo.end(); infoIter++ ) {
    int       key   = ( *infoIter ).first;
    ValuePair value = ( *infoIter ).second;
    eraseInfo( key );
    double scale = scalefactor[key];
    addInfo( key, value.first, value.second * scale );
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfo( const ExtraInfo& rhs ) {

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

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfo( const LongExtraInfo& rhs ) {

  // sum info or add new key
  LongExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int           key    = ( *iterB ).first;
    LongValuePair valueB = ( *iterB ).second;
    ValuePair     valueA = info( key, ValuePair( 0, 0 ) );
    if ( hasInfo( key ) ) { eraseInfo( key ); }
    addInfo( key, valueA.first + valueB.first, valueA.second + (double)valueB.second );
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfoNormalized( const ExtraInfo& rhs,
                                                                              const double     factor ) {

#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 1572 ) // Floating-point equality and inequality comparisons are unreliable
#  pragma warning( push )
#endif
  // sum info or add new key
  ExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int       key    = ( *iterB ).first;
    ValuePair valueB = ( *iterB ).second;
    ValuePair valueA = info( key, ValuePair( 0, 0 ) );
    if ( valueB.first ) {
      if ( hasInfo( key ) ) { eraseInfo( key ); }
      double scale = (double)valueA.first / (double)valueB.first;
      addInfo( key, valueA.first, valueA.second + factor * valueB.second * scale );
    }
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfoNormalized( const LongExtraInfo& rhs,
                                                                              const double         factor ) {

  // sum info or add new key
  LongExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int           key    = ( *iterB ).first;
    LongValuePair valueB = ( *iterB ).second;
    ValuePair     valueA = info( key, ValuePair( 0, 0 ) );
    if ( valueB.first ) {
      if ( hasInfo( key ) ) { eraseInfo( key ); }
      double scale = (double)valueA.first / (double)valueB.first;
      addInfo( key, valueA.first, valueA.second + factor * (double)valueB.second * scale );
    }
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfoNormalized( const ExtraInfo& rhs, const double factor,
                                                                              const int fromkey ) {

  // sum info or add new key
  ExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int       key    = ( *iterB ).first;
    ValuePair valueB = ( *iterB ).second;
    ValuePair valueA = info( key, ValuePair( 0, 0 ) );
    if ( valueB.first ) {
      if ( hasInfo( key ) ) { eraseInfo( key ); }

      // scale with the existing data except if factor == 0
      double scale;
      double usefactor;
      int    increment;
      double initial;
      if ( factor == 0 ) {
        scale     = 1.;
        usefactor = 1.;
        increment = valueB.first;
        initial   = 0;
      } else {
        scale     = (double)valueA.first / (double)valueB.first;
        usefactor = factor;
        increment = valueA.first;
        initial   = valueA.second;
      }

      // add normalized - use -log from the from key
      double addition;
      if ( key < fromkey ) {
        addition = usefactor * (double)valueB.second * scale;
      } else {
        double frac = (double)valueB.second / (double)valueB.first;
        if ( frac == 1.0 ) frac = 0.995;
        addition = usefactor * ( -1. ) * log( 1.0 - frac ) * (double)increment;
      }
      addInfo( key, increment, initial + addition );
    }
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::mergeInfoNormalized( const LongExtraInfo& rhs,
                                                                              const double factor, const int fromkey ) {

  // sum info or add new key
  LongExtraInfo::iterator iterB;
  for ( iterB = rhs.begin(); iterB != rhs.end(); iterB++ ) {
    int           key    = ( *iterB ).first;
    LongValuePair valueB = ( *iterB ).second;
    ValuePair     valueA = info( key, ValuePair( 0, 0 ) );
    if ( valueB.first ) {
      if ( hasInfo( key ) ) { eraseInfo( key ); }

      // scale with the existing data except if factor == 0
      double scale;
      double usefactor;
      int    increment;
      double initial;
      if ( factor == 0 ) {
        scale     = 1.;
        usefactor = 1.;
        increment = valueB.first;
        initial   = 0;
      } else {
        scale     = (double)valueA.first / (double)valueB.first;
        usefactor = factor;
        increment = valueA.first;
        initial   = valueA.second;
      }

      // add normalized - use -log from the from key
      double addition;
      if ( key < fromkey ) {
        addition = usefactor * (double)valueB.second * scale;
      } else {
        double frac = (double)valueB.second / (double)valueB.first;
        if ( frac == 1.0 ) frac = 0.995;
        addition = usefactor * ( -1. ) * log( 1.0 - frac ) * (double)increment;
      }
      addInfo( key, increment, initial + addition );
    }
  }
  return m_extraInfo;
}

inline LHCb::LumiIntegral::ExtraInfo LHCb::LumiIntegral::incrementInfo( const int key, const double count ) {

  // increment info or add new key, only if data exists
  if ( count != -1 ) {
    ValuePair value = info( key, ValuePair( 0, 0 ) );
    if ( hasInfo( key ) ) { eraseInfo( key ); }
    addInfo( key, ++value.first, value.second + count );
  }
  return m_extraInfo;
#ifdef __INTEL_COMPILER // End disable ICC remark from ROOT
#  pragma warning( pop )
#endif
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+( const LHCb::LumiIntegral& rhs ) {

  if ( this != &rhs ) {
    // merge run number sets
    mergeRuns( rhs.runNumbers() );
    // merge file ID sets
    mergeFileIDs( rhs.fileIDs() );
    // merge the extraInfo
    mergeInfo( rhs.extraInfo() );
  }
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+( const LHCb::LumiIntegral* rhs ) {

  return *this = *this + *rhs;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+=( const LHCb::LumiIntegral& rhs ) {

  if ( this != &rhs ) { *this = *this + rhs; }
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+=( const LHCb::LumiIntegral* rhs ) {

  if ( this != rhs ) { *this = *this + *rhs; }
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+( const LHCb::LumiFSR& rhs ) {

  // merge run number sets
  mergeRuns( rhs.runNumbers() );
  // merge file ID sets
  mergeFileIDs( rhs.fileIDs() );
  // merge the extraInfo
  mergeInfo( rhs.extraInfo() );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+( const LHCb::LumiFSR* rhs ) { return *this = *this + *rhs; }

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+=( const LHCb::LumiFSR& rhs ) { return *this + rhs; }

inline LHCb::LumiIntegral& LHCb::LumiIntegral::operator+=( const LHCb::LumiFSR* rhs ) { return *this + *rhs; }

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiIntegral& rhs, const double factor ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs.extraInfo(), factor );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiIntegral* rhs, const double factor ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs->extraInfo(), factor );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiFSR& rhs, const double factor ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs.extraInfo(), factor );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiFSR* rhs, const double factor ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs->extraInfo(), factor );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiIntegral& rhs, const double factor,
                                                              const int fromkey ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs.extraInfo(), factor, fromkey );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiIntegral* rhs, const double factor,
                                                              const int fromkey ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs->extraInfo(), factor, fromkey );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiFSR& rhs, const double factor,
                                                              const int fromkey ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs.extraInfo(), factor, fromkey );
  return *this;
}

inline LHCb::LumiIntegral& LHCb::LumiIntegral::addNormalized( const LHCb::LumiFSR* rhs, const double factor,
                                                              const int fromkey ) {

  // merge the extraInfo
  mergeInfoNormalized( rhs->extraInfo(), factor, fromkey );
  return *this;
}

inline std::ostream& LHCb::LumiIntegral::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ "
     << " runs : ";

  RunNumbers::const_iterator run;
  for ( run = m_runNumbers.begin(); run != m_runNumbers.end(); run++ ) { ss << *run << " "; }
  LHCb::LumiIntegral::FileIDs::const_iterator file;
  ss << " files : ";
  for ( file = m_fileIDs.begin(); file != m_fileIDs.end(); file++ ) { ss << *file << " "; }

  // overall sum info
  ss << " info (key/incr/integral) : ";
  ExtraInfo::const_iterator infoIter;
  for ( infoIter = m_extraInfo.begin(); infoIter != m_extraInfo.end(); infoIter++ ) {
    // get the key and value of the input info
    int                           key    = infoIter->first;
    LHCb::LumiIntegral::ValuePair values = infoIter->second;
    int                           incr   = values.first;
    double                        count  = values.second;
    ss << key << " " << incr << " " << count << " / ";
  }
  ss << " }";

  return ss;
}
