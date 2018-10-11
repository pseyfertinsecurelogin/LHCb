/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// DetDesc
#include "DetDesc/TabulatedProperty.h"

// GaudiKernel
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/GaudiException.h"

// STL
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
TabulatedProperty::TabulatedProperty( const std::string& /*Name*/ )
  : m_type    ( "(Unknown)" )
  , m_xAxis   ( "(empty)"   )
  , m_yAxis   ( "(empty)"   )
{}
////////////////////////////////////////////////////////////////////////////////
std::ostream& TabulatedProperty::fillStream ( std::ostream& s ) const
{
  s << " \tTabulatedProperty "
    << " \ttype="      << type ()
    << " \txAxis="     << xAxis()
    << " \tyAxis="     << yAxis()
    << " \t#enties="   << table().size() << std::endl;
  for( auto it = table().begin() ; table().end() != it ; ++it )
    {
      s << " \t\t\t( entry#"
        << std::setw(2) << it-table().begin()
        << "   "
        << std::setw(12) << it->first
        << "   "
        << std::setw(12) << it->second << " )\n";
    }
  return s ;
}
////////////////////////////////////////////////////////////////////////////////
MsgStream&    TabulatedProperty::fillStream ( MsgStream&    s ) const
{
  s << " \tTabulatedProperty "
    << " \ttype="       << type()
    << " \txAxis="      << xAxis()
    << " \tyAxis="      << yAxis()
    << " \t#entries="   << table().size() << endmsg;
  for( auto it = table().begin() ; table().end() != it ; ++it )
    {
      s << " \t\t\t( entry#"
        << std::setw(2) << it-table().begin()
        << "   "
        << std::setw(12) << it->first
        << "   "
        << std::setw(12) << it->second << " )" << endmsg ;
    }
  return s ;
}

////////////////////////////////////////////////////////////////////////////////

std::string TabulatedProperty::toXml(const std::string &name) const {
  std::ostringstream xml;
  // XML header
  xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE DDDB SYSTEM \"geometry.dtd\">";
  // DDDB open
  xml << "<DDDB>";
  // condition open
  xml << "<tabproperty name=\"";
  if (name.empty()) {
    if (registry()){
      xml << registry()->name();
    } else {
      xml << "TabulatedProperty";
    }
  } else {
    xml << name;
  }
  xml << "\" type=\"" << type();
  if ( !xAxis().empty() ) xml << "\" xaxis=\"" << xAxis();
  if ( !yAxis().empty() ) xml << "\" yaxis=\"" << yAxis();
  xml << "\">";

  // The table itself
  xml.precision(16); // precision for doubles
  Table::const_iterator i;
  for ( i = table().begin() ; i != table().end() ; ++i ) {
    xml << "<entry x = '" << i->first << "'  y = '" << i->second  << "' />";
  }

  // condition close
  xml << "</tabproperty>";
  // DDDB close
  xml << "</DDDB>\n";

  return xml.str();
}

//=========================================================================
//  Perform a deep copy from another TabulatedProperty
//=========================================================================
void TabulatedProperty::update ( ValidDataObject& obj ) {
  // first check the class
  TabulatedProperty *tp = dynamic_cast<TabulatedProperty *>(&obj);
  if (!tp){
    throw GaudiException("Trying to do a deep copy between different classes","TabulatedProperty::update",StatusCode::FAILURE);
  }
  // call the parent update method
  ValidDataObject::update(obj);

  // copy the internal data
  m_type  = tp->m_type;
  m_xAxis = tp->m_xAxis;
  m_yAxis = tp->m_yAxis;
  m_table = tp->m_table;

}
////////////////////////////////////////////////////////////////////////////////
