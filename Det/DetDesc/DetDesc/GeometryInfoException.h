#ifndef     DETDESC_GEOMETRYINFOEXCEPTION_H
#define     DETDESC_GEOMETRYINFOEXCEPTION_H 1
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

class GeometryInfoException : public GaudiException
{
public:
  /// constructor
  GeometryInfoException( const std::string  & name     ,
                         const IGeometryInfo * gi   = nullptr ,
                         const StatusCode   & sc = StatusCode::FAILURE );
  ///
  GeometryInfoException( const std::string    & name      ,
                         const GaudiException & ge        ,
                         const IGeometryInfo   * gi   =  nullptr ,
                         const StatusCode     & sc = StatusCode::FAILURE ) ;
  ///
  std::ostream& printOut( std::ostream& os = std::cerr ) const  override;
  MsgStream&    printOut( MsgStream&    os             ) const  override;
  ///
  GaudiException* clone() const override;
  ///
private:
  ///
  const IGeometryInfo*   m_gie_geometryInfo  ;
  ///
};
///

#endif  //  DETDESC_GEOMETRYINFOEXCEPTION_H
