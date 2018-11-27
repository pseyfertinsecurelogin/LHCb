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
#ifndef        __DETDESC_SOLID_SOLIDEXCEPTION_H__
#define        __DETDESC_SOLID_SOLIDEXCEPTION_H__
/// GaudiKernle
#include "GaudiKernel/GaudiException.h"
///
struct ISolid;
///

class SolidException : public GaudiException
{
  ///
public:
  ///
  SolidException( const std::string&     message      ,
                  const ISolid*          solid  =  nullptr  );
  ///
  SolidException( const std::string   &  message      ,
                  const GaudiException&  Exception    ,
                  const ISolid*          solid  =  nullptr  );
  ///
  std::ostream&   printOut( std::ostream& os = std::cerr ) const  override;
  MsgStream&      printOut( MsgStream&    os             ) const  override;
  GaudiException* clone   ()                               const  override;
  ///
 private:
  ///
  const ISolid*     m_se_solid         ;       // the "author" of the exception
  ///
};


#endif    //   __DETDESC_SOLID_SOLIDEXCEPTION_H__

