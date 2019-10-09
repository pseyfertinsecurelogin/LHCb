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
#include "Event/CrossSectionsFSR.h"
#include "Event/GenCountersFSR.h"
#include "GaudiKernel/DataObject.h"
#include "math.h"
#include "stdio.h"
#include <fstream>
#include <map>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_GenFSR = 13600;

  // Namespace for locations in TDS
  namespace GenFSRLocation {
    inline const std::string Default = "/FileRecords/GenFSR";
  }

  /** @class GenFSR GenFSR.h
   *
   * Accounting class for Generator in FSR
   *
   * @author Davide Fazzini
   *
   */

  class GenFSR final : public DataObject {
  public:
    /// Counter informations
    typedef std::pair<int, longlong> CounterValues;
    /// Generator counter
    typedef std::map<int, CounterValues> GeneratorCounter;
    /// Counter informations
    typedef std::pair<std::string, double> CrossValues;
    /// Cross-Section
    typedef std::map<int, CrossValues> CrossSection;
    /// Simulation string information container
    typedef std::map<std::string, std::string> StringInfo;
    /// Simulation int information container
    typedef std::map<std::string, int> IntInfo;

    /// Default Constructor
    GenFSR() : m_stringInfos(), m_intInfos(), m_genCounters(), m_crossSections() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Initialize the FSR information
    void initializeInfos();

    /// Get the int value of a FSR information
    int getSimulationInfo( const std::string& name, int def = 0 ) const;

    /// Get the string value of a FSR information
    std::string getSimulationInfo( const std::string& name, const std::string& def = "" ) const;

    /// Set the int value of a FSR information
    IntInfo addSimulationInfo( const std::string& name, int value );

    /// Set the string value of a FSR information
    StringInfo addSimulationInfo( const std::string& name, const std::string& value );

    /// Increment the number of jobs used to create the FSR
    void incrementNJobs( const int nJobs = 1 );

    /// get method
    GeneratorCounter& getGenCounters();

    /// add a Generator Counter to the set
    void addGenCounter( const int key, const longlong& value, const int incr = 1 );

    /// has information for specified Generator counter
    bool hasGenCounter( int key ) const;

    /// erase Generator counter from the set
    void eraseGenCounter( int key );

    /// extract the informations associated with the given key.
    CounterValues getGenCounterInfo( const int key ) const;

    /// sums the counter values for existing keys and inserts a new key if needed
    GeneratorCounter mergeGenCounters( const GeneratorCounter& rhs_counter );

    /// increments the values for existing keys and inserts a new key if needed
    GeneratorCounter incrementGenCounter( const int key, const longlong& value );

    /// sum a set of counters
    longlong sumGenCounters( std::vector<std::string>& counters ) const;

    /// Select the right counter to use as denominator to evaluate the fractions
    longlong getDenominator( const int key ) const;

    /// Calculate the efficiency or the fraction relative to a counter
    double getEfficiency( const longlong& num, const longlong& den, const double C = 1 ) const;

    /// Calculate the efficiency error
    double getEfficiencyError( const longlong& num, const longlong& den, const double C = 1,
                               const bool flag = true ) const;

    /// get method
    CrossSection& getCrossSections();

    /// add a Cross-Section to the set
    void addCrossSection( const int key, const CrossValues& value );

    /// has information for specified Cross-Section
    bool hasCrossSection( int key ) const;

    /// erase a Cross-Section from the set
    void eraseCrossSection( int key );

    /// extract the informations associated with the given key.
    CrossValues getCrossSectionInfo( int key ) const;

    /// sum the cross-sections for existing keys and inserts a new key if needed
    CrossSection mergeCrossSections( const CrossSection& rhs_cross, const GeneratorCounter& rhs_counter );

    /// Addition operator
    GenFSR& operator+( const LHCb::GenFSR& rhs );

    /// Addition operator
    GenFSR& operator+( const LHCb::GenFSR* rhs );

    /// Addition operator
    GenFSR& operator+=( const LHCb::GenFSR& rhs );

    /// Addition operator
    GenFSR& operator+=( const LHCb::GenFSR* rhs );

    /// printout
    std::ostream& fillStream( std::ostream& text ) const override;

    /// Retrieve const  Informations related to the simulation conditions
    const StringInfo& stringInfos() const;

    /// Update  Informations related to the simulation conditions
    void setStringInfos( const StringInfo& value );

    /// Retrieve const  Numerical information related to the simulation conditions
    const IntInfo& intInfos() const;

    /// Update  Numerical information related to the simulation conditions
    void setIntInfos( const IntInfo& value );

    /// Retrieve const  Set of Generator Level counter used in this job
    const GeneratorCounter& genCounters() const;

    /// Update  Set of Generator Level counter used in this job
    void setGenCounters( const GeneratorCounter& value );

    /// Retrieve const  Set of Cross-Sections
    const CrossSection& crossSections() const;

    /// Update  Set of Cross-Sections
    void setCrossSections( const CrossSection& value );

    friend std::ostream& operator<<( std::ostream& str, const GenFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    StringInfo       m_stringInfos;   ///< Informations related to the simulation conditions
    IntInfo          m_intInfos;      ///< Numerical information related to the simulation conditions
    GeneratorCounter m_genCounters;   ///< Set of Generator Level counter used in this job
    CrossSection     m_crossSections; ///< Set of Cross-Sections

  }; // class GenFSR

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::GenFSR::clID() const { return LHCb::GenFSR::classID(); }

inline const CLID& LHCb::GenFSR::classID() { return CLID_GenFSR; }

inline const LHCb::GenFSR::StringInfo& LHCb::GenFSR::stringInfos() const { return m_stringInfos; }

inline void LHCb::GenFSR::setStringInfos( const StringInfo& value ) { m_stringInfos = value; }

inline const LHCb::GenFSR::IntInfo& LHCb::GenFSR::intInfos() const { return m_intInfos; }

inline void LHCb::GenFSR::setIntInfos( const IntInfo& value ) { m_intInfos = value; }

inline const LHCb::GenFSR::GeneratorCounter& LHCb::GenFSR::genCounters() const { return m_genCounters; }

inline void LHCb::GenFSR::setGenCounters( const GeneratorCounter& value ) { m_genCounters = value; }

inline const LHCb::GenFSR::CrossSection& LHCb::GenFSR::crossSections() const { return m_crossSections; }

inline void LHCb::GenFSR::setCrossSections( const CrossSection& value ) { m_crossSections = value; }

inline void LHCb::GenFSR::initializeInfos() {

  std::vector<std::string> list{"evtType", "hardGenerator", "generationMethod", "decFiles", "nJobs"};
  for ( unsigned int i = 0; i < list.size(); i++ ) {
    if ( list[i] == "evtType" || list[i] == "nJobs" ) {
      int int_val = 0;
      m_intInfos.insert( std::pair<std::string, int>( list[i], int_val ) );
    } else {
      std::string string_val = "";
      m_stringInfos.insert( std::pair<std::string, std::string>( list[i], string_val ) );
    }
  }
}

inline int LHCb::GenFSR::getSimulationInfo( const std::string& name, int def ) const {

  IntInfo::const_iterator i = m_intInfos.find( name );
  return m_intInfos.end() == i ? def : i->second;
}

inline std::string LHCb::GenFSR::getSimulationInfo( const std::string& name, const std::string& def ) const {

  StringInfo::const_iterator i = m_stringInfos.find( name );
  return m_stringInfos.end() == i ? def : i->second;
}

inline LHCb::GenFSR::IntInfo LHCb::GenFSR::addSimulationInfo( const std::string& name, int value ) {

  m_intInfos[name] = value;
  return m_intInfos;
}

inline LHCb::GenFSR::StringInfo LHCb::GenFSR::addSimulationInfo( const std::string& name, const std::string& value ) {

  m_stringInfos[name] = value;
  return m_stringInfos;
}

inline void LHCb::GenFSR::incrementNJobs( const int nJobs ) {

  int info = getSimulationInfo( "nJobs", 0 );
  addSimulationInfo( "nJobs", info + nJobs );
}

inline LHCb::GenFSR::GeneratorCounter& LHCb::GenFSR::getGenCounters() { return m_genCounters; }

inline void LHCb::GenFSR::addGenCounter( const int key, const longlong& value, const int incr ) {

  CounterValues pair( incr, value );
  m_genCounters.insert( std::pair<int, CounterValues>( key, pair ) );
}

inline bool LHCb::GenFSR::hasGenCounter( int key ) const {

  return ( m_genCounters.end() != m_genCounters.find( key ) );
}

inline void LHCb::GenFSR::eraseGenCounter( int key ) { m_genCounters.erase( key ); }

inline LHCb::GenFSR::CounterValues LHCb::GenFSR::getGenCounterInfo( const int key ) const {

  CounterValues                    def( 0, 0 );
  GeneratorCounter::const_iterator i = m_genCounters.find( key );
  return m_genCounters.end() == i ? def : i->second;
}

inline LHCb::GenFSR::GeneratorCounter LHCb::GenFSR::mergeGenCounters( const GeneratorCounter& rhs_counter ) {

  // sum counter values or add new key
  GeneratorCounter::const_iterator icounter;
  for ( icounter = rhs_counter.begin(); icounter != rhs_counter.end(); icounter++ ) {
    int           key      = ( *icounter ).first;
    CounterValues counterB = ( *icounter ).second;
    CounterValues counterA = getGenCounterInfo( key );
    if ( hasGenCounter( key ) ) { eraseGenCounter( key ); }
    addGenCounter( key, counterA.second + counterB.second, counterA.first + counterB.first );
  }

  return m_genCounters;
}

inline LHCb::GenFSR::GeneratorCounter LHCb::GenFSR::incrementGenCounter( const int key, const longlong& value ) {

  // increment counter or add new key, only if data exists
  if ( value != -1 ) {
    CounterValues pair( 0, 0 );
    if ( hasGenCounter( key ) ) {
      pair = getGenCounterInfo( key );
      eraseGenCounter( key );
    }

    addGenCounter( key, pair.second + value, pair.first + 1 );
  }

  return m_genCounters;
}

inline longlong LHCb::GenFSR::sumGenCounters( std::vector<std::string>& counters ) const {

  longlong sum    = 0;
  int      length = counters.size();
  int      key    = 0;

  for ( int i = 0; i < length; i++ ) {
    key = GenCountersFSR::CounterKeyToType( counters[i] );
    sum += getGenCounterInfo( key ).second;
  }
  return sum;
}

inline longlong LHCb::GenFSR::getDenominator( const int key ) const {

  longlong den = 0;

  if ( hasGenCounter( key ) ) {
    CounterValues Acounter( 0, 0 ), Bcounter( 0, 0 );
    bool          sum_flag     = false;
    std::string   keyName      = " ";
    std::string   BcounterName = " ";

    Acounter = getGenCounterInfo( key );
    keyName  = GenCountersFSR::CounterKeyToString( key );

    // For these counters the fraction is calculated using the sum of a sub-set of counters
    if ( ( key >= 30 && key <= 50 ) || // B Hadron counters
         ( key >= 55 && key <= 71 ) || // D Hadron counters
         ( key >= 75 && key <= 80 ) || // B** Hadron counters
         ( key >= 85 && key <= 90 ) || // D** Hadron counters
         ( key == 91 || key == 92 ) )  // signal/anti-signal counters
      sum_flag = true;

    if ( !sum_flag ) {
      if ( keyName == "EvtGenerated" ) {
        BcounterName = "EvtGenerated";
      } else if ( keyName == "IntGenerated" )
        BcounterName = "AllEvt";
      else if ( keyName == "IntAccepted" )
        BcounterName = "EvtAccepted";
      else if ( keyName == "AfterFullEvt" )
        BcounterName = "BeforeFullEvt";
      else if ( key >= 10 && key <= 16 ) // counters of generated interactions
      {
        BcounterName = "IntGenerated";
      } else if ( key >= 17 && key <= 23 ) // counters of accepted interactions
      {
        BcounterName = "IntAccepted";
      } else if ( keyName == "AfterLevelCut" )
        BcounterName = "BeforeLevelCut";
      else if ( keyName == "AfterPCut" )
        BcounterName = "BeforePCut";
      else if ( keyName == "AfterantiPCut" )
        BcounterName = "BeforeantiPCut";
      else
        BcounterName = "Unknown";

      if ( BcounterName != "Unknown" ) {
        Bcounter = getGenCounterInfo( GenCountersFSR::CounterKeyToType( BcounterName ) );
        den      = Bcounter.second;
      }
    } else if ( sum_flag ) {
      std::vector<std::string> counters_vec;

      if ( keyName == "B0Gen" || keyName == "BplusGen" || keyName == "Bs0Gen" || keyName == "bBaryonGen" ||
           keyName == "BcplusGen" ) {
        std::string counters[] = {"B0Gen", "BplusGen", "Bs0Gen", "bBaryonGen", "BcplusGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "antiB0Gen" || keyName == "BminusGen" || keyName == "antiBs0Gen" ||
                  keyName == "antibBaryonGen" || keyName == "BcminusGen" ) {
        std::string counters[] = {"antiB0Gen", "BminusGen", "antiBs0Gen", "antibBaryonGen", "BcminusGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "B0Acc" || keyName == "BplusAcc" || keyName == "Bs0Acc" || keyName == "bBaryonAcc" ||
                  keyName == "BcplusAcc" ) {
        std::string counters[] = {"B0Acc", "BplusAcc", "Bs0Acc", "bBaryonAcc", "BcplusAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "antiB0Acc" || keyName == "BminusAcc" || keyName == "antiBs0Acc" ||
                  keyName == "antibBaryonAcc" || keyName == "BcminusAcc" ) {
        std::string counters[] = {"antiB0Acc", "BminusAcc", "antiBs0Acc", "antibBaryonAcc", "BcminusAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "D0Gen" || keyName == "DplusGen" || keyName == "DsplusGen" || keyName == "cBaryonGen" ) {
        std::string counters[] = {"D0Gen", "DplusGen", "DsplusGen", "cBaryonGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "antiD0Gen" || keyName == "DminusGen" || keyName == "DsminusGen" ||
                  keyName == "anticBaryonGen" ) {
        std::string counters[] = {"antiD0Gen", "DminusGen", "DsminusGen", "anticBaryonGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "D0Acc" || keyName == "DplusAcc" || keyName == "DsplusAcc" || keyName == "cBaryonAcc" ) {
        std::string counters[] = {"D0Acc", "DplusAcc", "DsplusAcc", "cBaryonAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "antiD0Acc" || keyName == "DminusAcc" || keyName == "DsminusAcc" ||
                  keyName == "anticBaryonAcc" ) {
        std::string counters[] = {"antiD0Acc", "DminusAcc", "DsminusAcc", "anticBaryonAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "BGen" || keyName == "BstarGen" || keyName == "B2starGen" ) {
        std::string counters[] = {"BGen", "BstarGen", "B2starGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "DGen" || keyName == "DstarGen" || keyName == "D2starGen" ) {
        std::string counters[] = {"DGen", "DstarGen", "D2starGen"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "BAcc" || keyName == "BstarAcc" || keyName == "B2starAcc" ) {
        std::string counters[] = {"BAcc", "BstarAcc", "B2starAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "DAcc" || keyName == "DstarAcc" || keyName == "D2starAcc" ) {
        std::string counters[] = {"DAcc", "DstarAcc", "D2starAcc"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      } else if ( keyName == "EvtSignal" || keyName == "EvtantiSignal" ) {
        std::string counters[] = {"EvtSignal", "EvtantiSignal"};
        counters_vec.assign( counters, counters + sizeof( counters ) / sizeof( counters[0] ) );
      }

      den = sumGenCounters( counters_vec );
    }
  }

  return den;
}

inline double LHCb::GenFSR::getEfficiency( const longlong& num, const longlong& den, const double C ) const {

  if ( num != 0 && den != 0 ) {
    double efficiency = C * ( (double)num / (double)den );
    return efficiency;
  } else {
    double efficiency = 0;
    return efficiency;
  }
}

inline double LHCb::GenFSR::getEfficiencyError( const longlong& num, const longlong& den, const double C,
                                                const bool flag ) const {

  if ( num != 0 && den != 0 ) {
    double error = 0;

    if ( flag == true ) {
      error = C * sqrt( (double)num * ( 1 - ( (double)num / (double)den ) ) ) / ( (double)den );
    } else {
      error = C * sqrt( (double)num * ( 1 + ( (double)num / (double)den ) ) ) / ( (double)den );
    }

    return error;
  } else {
    double error = 0;
    return error;
  }
}

inline LHCb::GenFSR::CrossSection& LHCb::GenFSR::getCrossSections() { return m_crossSections; }

inline void LHCb::GenFSR::addCrossSection( const int key, const CrossValues& value ) {

  m_crossSections.insert( std::pair<int, CrossValues>( key, value ) );
}

inline bool LHCb::GenFSR::hasCrossSection( int key ) const {

  return ( m_crossSections.end() != m_crossSections.find( key ) );
}

inline void LHCb::GenFSR::eraseCrossSection( int key ) { m_crossSections.erase( key ); }

inline LHCb::GenFSR::CrossValues LHCb::GenFSR::getCrossSectionInfo( int key ) const {

  CrossValues                  def( "", 0 );
  CrossSection::const_iterator i = m_crossSections.find( key );
  return m_crossSections.end() == i ? def : i->second;
}

inline LHCb::GenFSR::CrossSection LHCb::GenFSR::mergeCrossSections( const CrossSection&     rhs_cross,
                                                                    const GeneratorCounter& rhs_counter ) {

  // sum cross-sections or add new key
  CrossSection::const_iterator icross;
  for ( icross = rhs_cross.begin(); icross != rhs_cross.end(); icross++ ) {
    int    keys   = ( *icross ).first;
    double crossB = ( *icross ).second.second;
    double crossA = getCrossSectionInfo( keys ).second;

    GeneratorCounter::const_iterator icounter;
    int                              keyc   = 0;
    longlong                         countA = 0, countB = 0;
    std::string                      name = ( *icross ).second.first;

    for ( icounter = rhs_counter.begin(); icounter != rhs_counter.end(); icounter++ ) {
      keyc = ( *icounter ).first;
      if ( keyc == keys + 100 ) {
        countB = ( *icounter ).second.second;
        break;
      }
    }

    countA = getGenCounterInfo( keyc ).second;

    if ( hasCrossSection( keys ) ) eraseCrossSection( keys );

    CrossValues meanCross( "", 0 );

    if ( countB == 0 && countA == 0 )
      meanCross = {name, 0};
    else
      meanCross = {name, ( crossA * countA + crossB * countB ) / ( countA + countB )};

    addCrossSection( keys, meanCross );
  }

  return m_crossSections;
}

inline LHCb::GenFSR& LHCb::GenFSR::operator+( const LHCb::GenFSR& rhs ) {

  if ( this != &rhs ) {
    // merge the cross sections
    mergeCrossSections( rhs.m_crossSections, rhs.m_genCounters );

    // merge the generator level counters
    mergeGenCounters( rhs.m_genCounters );

    // increment the number of jobs
    incrementNJobs( rhs.getSimulationInfo( "nJobs", 0 ) );
  }

  return *this;
}

inline LHCb::GenFSR& LHCb::GenFSR::operator+( const LHCb::GenFSR* rhs ) {

  if ( this != rhs ) {
    // merge the cross sections
    mergeCrossSections( rhs->m_crossSections, rhs->m_genCounters );

    // merge the generator level counters
    mergeGenCounters( rhs->m_genCounters );

    // increment the number of jobs
    incrementNJobs( rhs->getSimulationInfo( "nJobs", 0 ) );
  }

  return *this;
}

inline LHCb::GenFSR& LHCb::GenFSR::operator+=( const LHCb::GenFSR& rhs ) { return *this + rhs; }

inline LHCb::GenFSR& LHCb::GenFSR::operator+=( const LHCb::GenFSR* rhs ) { return *this + *rhs; }

inline std::ostream& LHCb::GenFSR::fillStream( std::ostream& text ) const {

  std::ostream& s = const_cast<std::ostream&>( text );

  s << "{ "
    << "Event type: " << getSimulationInfo( "evtType", 0 );
  s << "  Hard Generator: " << getSimulationInfo( "hardGenerator", "" );
  s << "  Generation method: " << getSimulationInfo( "generationMethod", "" );
  s << "  DecFiles version: " << getSimulationInfo( "decFiles", "" );
  s << "  njobs:" << getSimulationInfo( "nJobs", 0 );
  s << std::endl << std::endl;
  s << " generator counters: ";
  GeneratorCounter::const_iterator icounter;
  for ( icounter = m_genCounters.begin(); icounter != m_genCounters.end(); icounter++ ) {
    int         key   = icounter->first;
    longlong    value = icounter->second.second;
    std::string name  = LHCb::GenCountersFSR::CounterKeyToString( key );
    if ( name == "Unknown" ) {
      CrossSection::const_iterator i = m_crossSections.find( key - 100 );
      name                           = i->second.first;
    }
    s << name << "  key: " << key << "  value: " << value << " / ";
  }
  s << std::endl << std::endl;

  s << "cross-sections: ";
  CrossSection::const_iterator icross;
  for ( icross = m_crossSections.begin(); icross != m_crossSections.end(); icross++ ) {
    int         key   = icross->first;
    double      value = icross->second.second;
    std::string name  = icross->second.first;
    name.erase( name.size() - 2 );

    s << name << "  key: " << key << "  value: " << value << " / ";
  }
  s << " }";

  return s;
}
