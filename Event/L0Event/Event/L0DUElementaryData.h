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
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SerializeSTL.h"
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace L0DUElementaryDataLocation {
    inline const std::string Default  = "Trig/L0/L0DUData";
    inline const std::string Emulated = "Emu/L0/L0DUData";
  } // namespace L0DUElementaryDataLocation

  /** @class L0DUElementaryData L0DUElementaryData.h
   *
   * This class represents the L0DU Data
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUElementaryData final : public ContainedObject {
  public:
    /// typedef for ObjectVector of L0DUElementaryData
    typedef ObjectVector<L0DUElementaryData> Container;

    /// std::map Container
    typedef std::map<std::string, L0DUElementaryData*> Map;

    /// Types of ElementaryData
    enum Type { Predefined = 0, Constant, Compound, RAMBcid };

    /// non-default constructor (for single data)
    L0DUElementaryData( int id, const LHCb::L0DUElementaryData::Type& type, const std::string& name, double scale = 1.,
                        int saturation = 0. );

    /// non-default constructor (for compound data)
    L0DUElementaryData( int id, const std::string& name, const std::string& op, const std::vector<std::string>& names );

    /// Default Constructor
    L0DUElementaryData()
        : m_id( 0 )
        , m_type()
        , m_name()
        , m_digit()
        , m_scale( 1. )
        , m_saturation( 0 )
        , m_evaluated( false )
        , m_operator( "" )
        , m_componentsName()
        , m_components() {}

    /// Copy Constructor
    L0DUElementaryData( const L0DUElementaryData& rh );

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// get operator symbol for compound data
    std::string getOperator();

    /// clear data and reset status
    void clear();

    /// add component Name
    void addComponent( const std::string& name );

    /// add component data
    void addComponent( LHCb::L0DUElementaryData* data );

    /// set digit scale and saturation
    void setScaleAndSaturation( double scale, unsigned int saturation );

    /// get digit scale
    double scale();

    /// get digit saturation
    unsigned int saturation();

    /// set digit value for a given bx, scale and saturation
    void setDigit( const unsigned int value, double scale, unsigned int saturation, int bx = 0 );

    /// set digit value for given bx
    void setDigit( const unsigned int value, int bx = 0 );

    /// set digit map
    void setDigit( const std::map<int, unsigned int>& map );

    /// get digit value for given bx
    unsigned int digit( int bx = 0 );

    /// scaled data  value
    double value( int bx = 0 );

    /// L0DUElementaryData short description
    std::string description( int bx = 0 );

    /// L0DUElementaryData summary for a given bx
    std::string summary( int bx );

    /// L0DUElementaryData summary
    std::string summary();

    /// Compound evaluation for a given BX
    void evaluate( int bx = 0 );

    /// data status
    bool hasData( int bx = 0 ) const;

    /// Retrieve const   L0DU data index
    int id() const;

    /// Update   L0DU data index
    void setId( int value );

    /// Retrieve const   data type (predefined [0]/constant value [1]/user-defined [2])
    const LHCb::L0DUElementaryData::Type& type() const;

    /// Update   data type (predefined [0]/constant value [1]/user-defined [2])
    void setType( const LHCb::L0DUElementaryData::Type& value );

    /// Retrieve const   L0DU data name
    const std::string& name() const;

    /// Update   L0DU data name
    void setName( const std::string& value );

    /// Update   L0DU data value scale
    void setScale( double value );

    /// Update   L0DU data digit saturation
    void setSaturation( unsigned int value );

    /// Update   Operator symbol
    void setOperator( const std::string& value );

    /// Retrieve const   Compound components name
    const std::vector<std::string>& componentsName() const;

    /// Retrieve   Compound components name
    std::vector<std::string>& componentsName();

    /// Update   Compound components name
    void setComponentsName( const std::vector<std::string>& value );

    /// Retrieve const   Compound components
    const std::vector<LHCb::L0DUElementaryData*>& components() const;

    /// Retrieve   Compound components
    std::vector<LHCb::L0DUElementaryData*>& components();

    /// Update   Compound components
    void setComponents( const std::vector<LHCb::L0DUElementaryData*>& value );

    friend std::ostream& operator<<( std::ostream& str, const L0DUElementaryData& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    int                                    m_id;    ///<  L0DU data index
    LHCb::L0DUElementaryData::Type         m_type;  ///<  data type (predefined [0]/constant value [1]/user-defined [2])
    std::string                            m_name;  ///<  L0DU data name
    std::map<int, unsigned int>            m_digit; ///<  L0DU data digit per BX
    double                                 m_scale; ///<  L0DU data value scale
    unsigned int                           m_saturation;     ///<  L0DU data digit saturation
    bool                                   m_evaluated;      ///<  L0DU data evaluation status
    std::string                            m_operator;       ///<  Operator symbol
    std::vector<std::string>               m_componentsName; ///<  Compound components name
    std::vector<LHCb::L0DUElementaryData*> m_components;     ///<  Compound components

  }; // class L0DUElementaryData

  /// Definition of vector container type for L0DUElementaryData
  typedef ObjectVector<L0DUElementaryData> L0DUElementaryDatas;

  inline std::ostream& operator<<( std::ostream& s, LHCb::L0DUElementaryData::Type e ) {
    switch ( e ) {
    case LHCb::L0DUElementaryData::Predefined:
      return s << "Predefined";
    case LHCb::L0DUElementaryData::Constant:
      return s << "Constant";
    case LHCb::L0DUElementaryData::Compound:
      return s << "Compound";
    case LHCb::L0DUElementaryData::RAMBcid:
      return s << "RAMBcid";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::L0DUElementaryData::Type";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0DUElementaryData::L0DUElementaryData( int id, const LHCb::L0DUElementaryData::Type& type,
                                                     const std::string& name, double scale, int saturation )
    : m_id( id ), m_type( type ), m_name( name ), m_scale( scale ), m_saturation( saturation ), m_evaluated( false ) {

  m_operator = "";
}

inline LHCb::L0DUElementaryData::L0DUElementaryData( int id, const std::string& name, const std::string& op,
                                                     const std::vector<std::string>& names )
    : m_id( id )
    , m_type( Type::Compound )
    , m_name( name )
    , m_scale( 1. )
    , m_saturation( 0 )
    , m_evaluated( false )
    , m_operator( op ) {

  m_componentsName = names;
}

inline LHCb::L0DUElementaryData::L0DUElementaryData( const LHCb::L0DUElementaryData& rh )
    : ContainedObject( rh )
    , m_id( rh.m_id )
    , m_type( rh.m_type )
    , m_name( rh.m_name )
    , m_digit( rh.m_digit )
    , m_scale( rh.m_scale )
    , m_saturation( rh.m_saturation )
    , m_evaluated( rh.m_evaluated )
    , m_operator( rh.m_operator )
    , m_componentsName( rh.m_componentsName )
    , m_components( rh.m_components ) {}

inline std::ostream& LHCb::L0DUElementaryData::fillStream( std::ostream& s ) const {
  char l_evaluated = ( m_evaluated ) ? 'T' : 'F';
  s << "{ "
    << "id :	" << m_id << std::endl
    << "type :	" << m_type << std::endl
    << "name :	" << m_name << std::endl
    << "digit :	" << m_digit << std::endl
    << "scale :	" << m_scale << std::endl
    << "saturation :	" << m_saturation << std::endl
    << "evaluated :	" << l_evaluated << std::endl
    << "operator :	" << m_operator << std::endl
    << "componentsName :	" << m_componentsName << std::endl
    << "components :	" << m_components << std::endl
    << " }";
  return s;
}

inline int LHCb::L0DUElementaryData::id() const { return m_id; }

inline void LHCb::L0DUElementaryData::setId( int value ) { m_id = value; }

inline const LHCb::L0DUElementaryData::Type& LHCb::L0DUElementaryData::type() const { return m_type; }

inline void LHCb::L0DUElementaryData::setType( const LHCb::L0DUElementaryData::Type& value ) { m_type = value; }

inline const std::string& LHCb::L0DUElementaryData::name() const { return m_name; }

inline void LHCb::L0DUElementaryData::setName( const std::string& value ) { m_name = value; }

inline void LHCb::L0DUElementaryData::setScale( double value ) { m_scale = value; }

inline void LHCb::L0DUElementaryData::setSaturation( unsigned int value ) { m_saturation = value; }

inline void LHCb::L0DUElementaryData::setOperator( const std::string& value ) { m_operator = value; }

inline const std::vector<std::string>& LHCb::L0DUElementaryData::componentsName() const { return m_componentsName; }

inline std::vector<std::string>& LHCb::L0DUElementaryData::componentsName() { return m_componentsName; }

inline void LHCb::L0DUElementaryData::setComponentsName( const std::vector<std::string>& value ) {
  m_componentsName = value;
}

inline const std::vector<LHCb::L0DUElementaryData*>& LHCb::L0DUElementaryData::components() const {
  return m_components;
}

inline std::vector<LHCb::L0DUElementaryData*>& LHCb::L0DUElementaryData::components() { return m_components; }

inline void LHCb::L0DUElementaryData::setComponents( const std::vector<LHCb::L0DUElementaryData*>& value ) {
  m_components = value;
}

inline std::string LHCb::L0DUElementaryData::getOperator() { return m_operator; }

inline void LHCb::L0DUElementaryData::clear() {

  if ( m_type == Type::Constant ) return;
  m_components.clear();
  m_digit.clear();
  m_evaluated = false;
}

inline void LHCb::L0DUElementaryData::addComponent( const std::string& name ) { m_componentsName.push_back( name ); }

inline void LHCb::L0DUElementaryData::addComponent( LHCb::L0DUElementaryData* data ) { m_components.push_back( data ); }

inline void LHCb::L0DUElementaryData::setScaleAndSaturation( double scale, unsigned int saturation ) {

  m_scale      = scale;
  m_saturation = saturation;
}

inline double LHCb::L0DUElementaryData::scale() {

  if ( m_type == Type::Compound ) evaluate();
  return m_scale;
}

inline unsigned int LHCb::L0DUElementaryData::saturation() {

  if ( m_type == Type::Compound ) evaluate();
  return m_saturation;
}

inline void LHCb::L0DUElementaryData::setDigit( const unsigned int value, double scale, unsigned int saturation,
                                                int bx ) {

  if ( m_type != Type::Compound ) m_evaluated = true;
  setDigit( value, bx );
  setScaleAndSaturation( scale, saturation );
}

inline void LHCb::L0DUElementaryData::setDigit( const unsigned int value, int bx ) {

  if ( m_type != Type::Compound ) m_evaluated = true;
  if ( m_type == Type::Constant ) m_digit[0] = value;
  if ( m_type == Type::RAMBcid ) m_digit[0] = value - bx;
  m_digit[bx] = value;
}

inline void LHCb::L0DUElementaryData::setDigit( const std::map<int, unsigned int>& map ) {

  if ( m_type != Type::Compound ) m_evaluated = true;
  m_digit = map;
}

inline unsigned int LHCb::L0DUElementaryData::digit( int bx ) {

  // check if compound data
  if ( m_type == Type::Compound )
    evaluate( bx ); // evaluate compound
  else if ( m_type == Type::Constant )
    m_digit[bx] = m_digit[0]; // constant for any BX
  else if ( m_type == Type::RAMBcid )
    m_digit[bx] = m_digit[0] + bx; // RAMBcid bx shift
  if ( m_digit.end() == m_digit.find( bx ) ) return 0.;
  return m_digit[bx];
}

inline double LHCb::L0DUElementaryData::value( int bx ) { return ( (double)digit( bx ) ) * m_scale; }

inline bool LHCb::L0DUElementaryData::hasData( int bx ) const {

  return ( m_digit.find( bx ) != m_digit.end() && m_evaluated );
}
