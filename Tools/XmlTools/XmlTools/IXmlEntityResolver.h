#ifndef XMLTOOLS_IXMLENTITYRESOLVER_H 
#define XMLTOOLS_IXMLENTITYRESOLVER_H 1

// from Gaudi
#include "GaudiKernel/IInterface.h"

#include "xercesc/sax/EntityResolver.hpp"


/** Interface to allow a service or tool to provide a specialized xerces::EntityResolver.
 *  The XmlParserSvc.EntityResolverSvc must be set to the name of the service implementing this interface
 *  in order to use the specialized xerces::EntityResolver to handle special system IDs
 *  (like "conddb:/...").
 *
 *  @author Marco Clemencic
 *  @date   2005-10-12
 */
struct IXmlEntityResolver : extend_interfaces<IInterface> {
  DeclareInterfaceID(IXmlEntityResolver, 2, 0 );

  /// Return a pointer to the actual implementation of a xercesc::EntityResolver.
  virtual xercesc::EntityResolver *resolver() = 0;

};
#endif // XMLTOOLS_IXMLENTITYRESOLVERSVC_H
