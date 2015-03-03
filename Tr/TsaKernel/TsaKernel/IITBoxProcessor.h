#ifndef _IITBoxProcessor_H
#define _IITBoxProcessor_H

#include "GaudiKernel/IAlgTool.h"
#include "TsaKernel/SpacePoint.h"

static const InterfaceID IID_IITBoxProcessor( "IITBoxProcessor", 0, 0 );

class IITBoxProcessor: virtual public IAlgTool 
{
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_IITBoxProcessor ; }

  /// box type....
  virtual std::string boxType() const = 0;

  /// execute method
  virtual StatusCode execute(Tsa::SpacePointVector* pCont) = 0 ; 
};

#endif
