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
#ifndef DETDESC_GEOMETRYINFOEXCEPTION_H
#define DETDESC_GEOMETRYINFOEXCEPTION_H 1
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
///
class MsgStream;
struct IGeometryInfo;
///

/** @class GeometryInfoException GeometryInfoException.h DetDesc/GeometryInfoException.h

    Exception class used in GeometryInfo objects

    @author Vanya Belyaev


    Change to deal with IGeometryInfo

    @author Juan Palacios

*/

class GeometryInfoException : public GaudiException {
public:
  /// constructor
  GeometryInfoException( const std::string& name, const IGeometryInfo* gi = nullptr,
                         const StatusCode& sc = StatusCode::FAILURE );
  ///
  GeometryInfoException( const std::string& name, const GaudiException& ge, const IGeometryInfo* gi = nullptr,
                         const StatusCode& sc = StatusCode::FAILURE );
  ///
  std::ostream& printOut( std::ostream& os = std::cerr ) const override;
  MsgStream&    printOut( MsgStream& os ) const override;
  ///
  GaudiException* clone() const override;
  ///
private:
  ///
  const IGeometryInfo* m_gie_geometryInfo;
  ///
};
///

#endif //  DETDESC_GEOMETRYINFOEXCEPTION_H
