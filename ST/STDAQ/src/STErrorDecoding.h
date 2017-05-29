#ifndef STERRORDECODING_H
#define STERRORDECODING_H 1

#include <string>

#include "GaudiKernel/StatusCode.h"
//===========================================
// Include files
// from Gaudi
#include "STDecodingBaseAlg.h"

class ISvcLocator;


/** @class STErrorDecoding STErrorDecoding.h public/STErrorDecoding.h
 *
 *
 *  @author Mathias Knecht, M Needham
 *  @date   2007-09-11 (2008-06)
 */

class STErrorDecoding : public STDecodingBaseAlg {
public:
  /// Standard constructor
  STErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute   () override;    ///< Algorithm execution

private:

  bool m_PrintErrorInfo;

};
#endif // STERRORDECODING_H
