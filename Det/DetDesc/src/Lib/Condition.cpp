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
#include <string>

#include "DetDesc/Condition.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/IRegistry.h"

//---------------------------------------------------------------------------

/// Update using another instance of this class: deep copy all
/// contents, except for the properties of a generic DataObject
void Condition::update ( ValidDataObject& obj )
{
  /// See the implementation of ParamValidDataObject::update
  /// for a real implementation.
  ParamValidDataObject::update ( obj );
}

//=========================================================================
//  Prepare an XML string representing the condition
//=========================================================================
std::string Condition::toXml(std::string_view name, bool header, int precision) const{
  std::ostringstream xml;
  if (header) {
    // XML header
    xml << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
        << "<!DOCTYPE DDDB SYSTEM \"conddb:/DTD/structure.dtd\">";
    // DDDB open
    xml << "<DDDB>";
    // condition open
  }
  xml << "<condition classID=\"" << this->clID() << "\" name=\"";

  if (name.empty()) {
      name = ( registry() ? registry()->name() : "Condition" );
  }
  if ( name.front() == '/' ) name.remove_prefix(1);

  xml << name << "\">";

  // loop over parameters
  for ( const auto& i : paramNames() ) {
    xml << paramToString(i, precision);
  }

  // condition close
  xml << "</condition>";
  if (header) {
    // DDDB close
    xml << "</DDDB>";
  }

  return xml.str();
}
//---------------------------------------------------------------------------


/// initialize
StatusCode Condition::initialize() {
  // this is a default implementation that does nothing.
  // it is up to the user to override this in a child of Condition
  return StatusCode::SUCCESS;
}

//----------------------------------------------------------------------------
