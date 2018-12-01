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

#include "DetDesc/PVolume.h"
#include "DetDesc/PVolumeException.h"



/////////////////////////////////////////////////////////////////////////////////////////
PVolumeException::PVolumeException( const std::string& name    ,
                                    const PVolume*     pvolume )
  : GaudiException( name , "*PVolumeException*" , StatusCode::FAILURE )
  , m_pve_pvolume    ( pvolume    )
{}
/////////////////////////////////////////////////////////////////////////////////////////
PVolumeException::PVolumeException( const std::string&     name      ,
                                    const GaudiException&  Exception ,
                                    const PVolume*         pvolume     )
  : GaudiException( name , "*PVolumeException*" , StatusCode::FAILURE , Exception )
  , m_pve_pvolume    ( pvolume    )
{}
/////////////////////////////////////////////////////////////////////////////////////////
std::ostream& PVolumeException::printOut( std::ostream& os             ) const
{
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code()
     << " \t" << m_pve_pvolume << std::endl ;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) : os ;
  ///
}
//////////////////////////////////////////////////////////////////////////////////////////
MsgStream&    PVolumeException::printOut( MsgStream&    os             ) const
{
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code()
     << " \t" << m_pve_pvolume << endmsg;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) : os ;
  ///
}
//////////////////////////////////////////////////////////////////////////////////////////
GaudiException* PVolumeException::clone() const  { return  new PVolumeException(*this); }
/////////////////////////////////////////////////////////////////////////////////////////
