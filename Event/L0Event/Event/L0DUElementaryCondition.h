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
#include "Event/L0DUElementaryData.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SerializeSTL.h"
#include <map>
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace L0DUElementaryConditionLocation {
    inline const std::string Default  = "Trig/L0/L0DUConditions";
    inline const std::string Emulated = "Emu/L0/L0DUConditions";
  } // namespace L0DUElementaryConditionLocation

  /** @class L0DUElementaryCondition L0DUElementaryCondition.h
   *
   * This class represents the L0DU Condition
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUElementaryCondition final : public ContainedObject {
  public:
    /// typedef for ObjectVector of L0DUElementaryCondition
    typedef ObjectVector<L0DUElementaryCondition> Container;

    /// std::map Container
    typedef std::map<std::string, LHCb::L0DUElementaryCondition*> Map;

    /// non default constructor
    L0DUElementaryCondition( unsigned int id, const std::string& name, LHCb::L0DUElementaryData* data,
                             const std::string& comp, unsigned int threshold )
        : m_id( id )
        , m_name( name )
        , m_data( data )
        , m_comparator( comp )
        , m_threshold( threshold )
        , m_reported( true )
        , m_reportBit( id )
        , m_bx( 0 ) {}

    /// Default Constructor
    L0DUElementaryCondition()
        : m_id( 0 )
        , m_name()
        , m_data( 0 )
        , m_comparator()
        , m_threshold( 0 )
        , m_reported( true )
        , m_reportBit( 0 )
        , m_bx( 0 ) {}

    /// Copy Constructor
    L0DUElementaryCondition( const L0DUElementaryCondition& rh );

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// return bx to be considered
    int bx();

    /// Condition value = comparaison(data,comparator,threshold)
    bool emulatedValue();

    /// L0DUElementaryCondition short description
    std::string description();

    /// L0DUElementaryCondition full summary after processing
    std::string summary();

    /// Comparison method
    bool comparison( unsigned int a, const std::string& comparator, unsigned int b );

    /// Retrieve const   L0DU condition index
    unsigned int id() const;

    /// Update   L0DU condition index
    void setId( unsigned int value );

    /// Retrieve const   L0DU condition name
    const std::string& name() const;

    /// Update   L0DU condition name
    void setName( const std::string& value );

    /// Retrieve const   L0DU Elementary data
    const LHCb::L0DUElementaryData* data() const;

    /// Update   L0DU Elementary data
    void setData( LHCb::L0DUElementaryData* value );

    /// Retrieve const   comparator symbol
    const std::string& comparator() const;

    /// Update   comparator symbol
    void setComparator( const std::string& value );

    /// Retrieve const  threshold value (digit)
    unsigned int threshold() const;

    /// Update  threshold value (digit)
    void setThreshold( unsigned int value );

    /// Retrieve const  true if the condition-bit is reported the summaryReport
    bool reported() const;

    /// Update  true if the condition-bit is reported the summaryReport
    void setReported( bool value );

    /// Retrieve const  bit position in the  summaryReport == id() if all conditions are to be reported
    int reportBit() const;

    /// Update  bit position in the  summaryReport == id() if all conditions are to be reported
    void setReportBit( int value );

    /// Update  BX to be considered for the data value (default 0 = current BX)
    void setBx( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0DUElementaryCondition& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    unsigned int              m_id;         ///<  L0DU condition index
    std::string               m_name;       ///<  L0DU condition name
    LHCb::L0DUElementaryData* m_data;       ///<  L0DU Elementary data
    std::string               m_comparator; ///<  comparator symbol
    unsigned int              m_threshold;  ///< threshold value (digit)
    bool                      m_reported;   ///< true if the condition-bit is reported the summaryReport
    int m_reportBit; ///< bit position in the  summaryReport == id() if all conditions are to be reported
    int m_bx;        ///< BX to be considered for the data value (default 0 = current BX)

  }; // class L0DUElementaryCondition

  /// Definition of vector container type for L0DUElementaryCondition
  typedef ObjectVector<L0DUElementaryCondition> L0DUElementaryConditions;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0DUElementaryCondition::L0DUElementaryCondition( const LHCb::L0DUElementaryCondition& rh )
    : ContainedObject( rh )
    , m_id( rh.m_id )
    , m_name( rh.m_name )
    , m_data( rh.m_data )
    , m_comparator( rh.m_comparator )
    , m_threshold( rh.m_threshold )
    , m_reported( rh.m_reported )
    , m_reportBit( rh.m_reportBit )
    , m_bx( rh.m_bx ) {}

inline std::ostream& LHCb::L0DUElementaryCondition::fillStream( std::ostream& s ) const {
  char l_reported = ( m_reported ) ? 'T' : 'F';
  s << "{ "
    << "id :	" << m_id << std::endl
    << "name :	" << m_name << std::endl
    << "data :	" << m_data << std::endl
    << "comparator :	" << m_comparator << std::endl
    << "threshold :	" << m_threshold << std::endl
    << "reported :	" << l_reported << std::endl
    << "reportBit :	" << m_reportBit << std::endl
    << "bx :	" << m_bx << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::L0DUElementaryCondition::id() const { return m_id; }

inline void LHCb::L0DUElementaryCondition::setId( unsigned int value ) { m_id = value; }

inline const std::string& LHCb::L0DUElementaryCondition::name() const { return m_name; }

inline void LHCb::L0DUElementaryCondition::setName( const std::string& value ) { m_name = value; }

inline const LHCb::L0DUElementaryData* LHCb::L0DUElementaryCondition::data() const { return m_data; }

inline void LHCb::L0DUElementaryCondition::setData( LHCb::L0DUElementaryData* value ) { m_data = value; }

inline const std::string& LHCb::L0DUElementaryCondition::comparator() const { return m_comparator; }

inline void LHCb::L0DUElementaryCondition::setComparator( const std::string& value ) { m_comparator = value; }

inline unsigned int LHCb::L0DUElementaryCondition::threshold() const { return m_threshold; }

inline void LHCb::L0DUElementaryCondition::setThreshold( unsigned int value ) { m_threshold = value; }

inline bool LHCb::L0DUElementaryCondition::reported() const { return m_reported; }

inline void LHCb::L0DUElementaryCondition::setReported( bool value ) { m_reported = value; }

inline int LHCb::L0DUElementaryCondition::reportBit() const { return m_reportBit; }

inline void LHCb::L0DUElementaryCondition::setReportBit( int value ) { m_reportBit = value; }

inline void LHCb::L0DUElementaryCondition::setBx( int value ) { m_bx = value; }

inline int LHCb::L0DUElementaryCondition::bx() { return m_bx; }

inline bool LHCb::L0DUElementaryCondition::emulatedValue() {
  return comparison( m_data->digit( m_bx ), m_comparator, m_threshold );
}
