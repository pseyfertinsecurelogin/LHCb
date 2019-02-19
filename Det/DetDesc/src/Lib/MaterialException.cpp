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
/// GaudiKernbel
#include "GaudiKernel/MsgStream.h"
/// DetDesc
#include "DetDesc/Material.h"
#include "DetDesc/MaterialException.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////
MaterialException::MaterialException( const std::string& message, const Material* mat )
    : GaudiException( message, "*MaterialException*", StatusCode::FAILURE ), m_me_mat( mat ) {}
////////////////////////////////////////////////////////////////////////////////////////
MaterialException::MaterialException( const std::string& message, const GaudiException& Exception, const Material* mat )
    : GaudiException( message, "*MaterialException*", StatusCode::FAILURE, Exception ), m_me_mat( mat ) {}
////////////////////////////////////////////////////////////////////////////////////////
std::ostream& MaterialException::printOut( std::ostream& os ) const {
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << std::setw( 7 ) << code() << m_me_mat << std::endl;
  ///
  return previous() ? ( previous()->printOut( os ) ) : os;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////
MsgStream& MaterialException::printOut( MsgStream& os ) const {
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << std::setw( 7 ) << code() << m_me_mat << endmsg;
  ///
  return previous() ? ( previous()->printOut( os ) ) : os;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////
