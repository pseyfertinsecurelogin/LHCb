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
