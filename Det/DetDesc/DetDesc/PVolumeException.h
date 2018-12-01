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
#ifndef    DETDESC_PVOLUMEEXCEPTION_H
#define    DETDESC_PVOLUMEEXCEPTION_H 1
///
#include "GaudiKernel/GaudiException.h"
///
class PVolume;

///
class PVolumeException : public GaudiException
{
  ///
public:
  ///
  PVolumeException( const std::string& name          ,
                    const PVolume*     pvolume =  nullptr  ) ;
  ///
  PVolumeException( const std::string&     name          ,
                    const GaudiException&  Exception     ,
                    const PVolume*         pvolume =  nullptr  );
  ///
  std::ostream&   printOut ( std::ostream& os = std::cerr ) const  override;
  MsgStream&      printOut ( MsgStream&    os             ) const  override;
  GaudiException* clone    ()                               const  override;
  ///
private:
  ///
  const PVolume*    m_pve_pvolume    ;   // "author" of exceptiom
  ///
};
///


#endif  // DETDESC_PVOLUMEEXCEPTION_H











