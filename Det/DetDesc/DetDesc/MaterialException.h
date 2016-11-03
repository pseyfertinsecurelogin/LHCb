#ifndef    DETDESC_MATERIALEXCEPTION_H
#define    DETDESC_MATERIALEXCEPTION_H
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
//// STL
#include <iostream>
#include <exception>
//
class Material;
class MSgStream;
///

///
class MaterialException: public GaudiException
{
  ///
public:
  ///
  MaterialException( const std::string    & message     ,
                     const Material       * mat     = nullptr );
  ///
  MaterialException( const std::string    &  message    ,
                     const GaudiException &  Exception  ,
                     const Material       *  mat    = nullptr );
  ///
  std::ostream& printOut ( std::ostream& os = std::cerr ) const override;
  MsgStream&    printOut ( MsgStream&    os             ) const override;
  ///
  inline GaudiException* clone() const override {
    return new MaterialException( *this );
  }
  ///
private:
  ///
  const Material* m_me_mat;
  ///
};
///

#endif  // DETDESC_MATERIALEXCEPTION_H
