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
#include "DetDesc/ISolid.h"
#include "GaudiKernel/MsgStream.h"
// DetDesc
#include "DetDesc/SolidException.h"

//////////////////////////////////////////////////////////////////////////////////////
SolidException::SolidException( const std::string& message, const ISolid* solid )
    : GaudiException( message, "*SolidException*", StatusCode::FAILURE ), m_se_solid( solid ) {}
///////////////////////////////////////////////////////////////////////////////////////
SolidException::SolidException( const std::string& message, const GaudiException& Exception, const ISolid* solid )
    : GaudiException( message, "*SolidException*", StatusCode::FAILURE, Exception ), m_se_solid( solid ) {}
///////////////////////////////////////////////////////////////////////////////////////
std::ostream& SolidException::printOut( std::ostream& os ) const {
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code() << " \tSolid=" << m_se_solid << std::endl;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) : os;
  ///
}
///////////////////////////////////////////////////////////////////////////////////////
MsgStream& SolidException::printOut( MsgStream& os ) const {
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code() << " \tSolid=" << m_se_solid << endmsg;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) : os;
  ///
}
///////////////////////////////////////////////////////////////////////////////////////
GaudiException* SolidException::clone() const { return new SolidException( *this ); }
///////////////////////////////////////////////////////////////////////////////////////
