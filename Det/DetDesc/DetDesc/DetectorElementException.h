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
#ifndef       DETDESC_DetectorElementEXCEPTION_H
#define       DETDESC_DetectorElementEXCEPTION_H 1
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
//
class DetectorElement;
class MsgStream;

/** @class DetectorElementException DetectorElementException.h DetDesc/DetectorElementException.h

    Exception used in DetectorElement class

    @author Vanya Belyaev
*/



class DetectorElementException : public GaudiException
{
  ///
public:
  /// constructor
  DetectorElementException( const std::string     & name                                   ,
                            const DetectorElement * DetectorElement = nullptr              ,
                            const StatusCode      &  sc             = StatusCode::FAILURE  );
  /// constructor from exception
  DetectorElementException( const std::string     & name                                    ,
                            const GaudiException  & Exception                               ,
                            const DetectorElement * DetectorElement   = nullptr             ,
                            const StatusCode      & sc                = StatusCode::FAILURE  );
  ///
  std::ostream&    printOut ( std::ostream& os = std::cerr ) const  override;
  ///
  MsgStream&       printOut ( MsgStream&                   ) const  override;
  ///
  GaudiException*  clone    ()                               const  override;
  ///
 private:
  ///
  const DetectorElement*    m_dee_DetectorElement    ;
  ///
};
///

#endif  //    DETDESC_DetectorElementEXCEPTION_H
