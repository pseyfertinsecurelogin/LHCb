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
/// GaudiKernel
#include "DetDesc/GeometryInfoException.h"
#include "DetDesc/IGeometryInfo.h"
#include "GaudiKernel/MsgStream.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
GeometryInfoException::GeometryInfoException( const std::string& name, const IGeometryInfo* gi, const StatusCode& sc )
    : GaudiException( name, "*GeometryInfoException*", sc ), m_gie_geometryInfo( gi ) {}
///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryInfoException::GeometryInfoException( const std::string& name, const GaudiException& ge,
                                              const IGeometryInfo* gi, const StatusCode& sc )
    : GaudiException( name, "*GeometryInfoException*", sc, ge ), m_gie_geometryInfo( gi ) {}
////////////////////////////////////////////////////////////////////////////////////////////////////
GaudiException* GeometryInfoException::clone() const { return new GeometryInfoException( *this ); }
////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& GeometryInfoException::printOut( std::ostream& os ) const {
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << code() << m_gie_geometryInfo << std::endl;
  ///
  return previous() ? ( previous()->printOut( os ) ) : os;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
MsgStream& GeometryInfoException::printOut( MsgStream& os ) const {
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << code() << m_gie_geometryInfo << endmsg;
  ///
  return previous() ? ( previous()->printOut( os ) ) : os;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
