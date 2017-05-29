// $Id: createODIN.h,v 1.1 2008-01-15 10:11:23 cattanem Exp $
#ifndef CREATEODIN_H
#define CREATEODIN_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class IEventTimeDecoder;
class ISvcLocator;

/** @class createODIN createODIN.h
 *  Trivial algorithm to create DAQ/ODIN object from ODIN RawEvent bank
 *
 *  @author Marco Cattaneo
 *  @date   2008-01-15
 */
class createODIN final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  createODIN( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~createODIN( ) = default; ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  IEventTimeDecoder* m_decoder = nullptr; ///< Pointer to tool to decode ODIN bank

};
#endif // CREATEODIN_H
