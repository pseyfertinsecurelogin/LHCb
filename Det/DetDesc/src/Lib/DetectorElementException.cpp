/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// GaudiKernel
#include "GaudiKernel/MsgStream.h"
// DetDesc
#include "DetDesc/DetectorElement.h"
#include "DetDesc/DetectorElementException.h"

//////////////////////////////////////////////////////////////////////////////
DetectorElementException::DetectorElementException( const std::string& name, const DetectorElement* DE,
                                                    const StatusCode& sc )
    : GaudiException( name, "*DetElemException*", sc ), m_dee_DetectorElement( DE ) {}
//////////////////////////////////////////////////////////////////////////////
DetectorElementException::DetectorElementException( const std::string& name, const GaudiException& Exception,
                                                    const DetectorElement* DE, const StatusCode& sc )
    : GaudiException( name, "*DetElemException*", sc, Exception ), m_dee_DetectorElement( DE ) {}
//////////////////////////////////////////////////////////////////////////////
GaudiException* DetectorElementException::clone() const { return new DetectorElementException( *this ); }
//////////////////////////////////////////////////////////////////////////////
std::ostream& DetectorElementException::printOut( std::ostream& os ) const {
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code();
  if ( 0 != m_dee_DetectorElement ) {
    os << "\t for DetectorElement=";
    m_dee_DetectorElement->printOut( os );
  } else {
    os << "\t for UKNNOWN DetectorElement";
  }
  ///
  return previous() ? ( previous()->printOut( os ) ) : ( os << std::endl );
}
//////////////////////////////////////////////////////////////////////////////
MsgStream& DetectorElementException::printOut( MsgStream& os ) const {
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code();
  if ( 0 != m_dee_DetectorElement ) {
    os << "\t for DetectorElement=";
    m_dee_DetectorElement->printOut( os );
  } else {
    os << "\t for UKNNOWN DetectorElement";
  }
  ///
  return previous() ? ( previous()->printOut( os ) ) : ( os << endmsg );
}
//////////////////////////////////////////////////////////////////////////////
