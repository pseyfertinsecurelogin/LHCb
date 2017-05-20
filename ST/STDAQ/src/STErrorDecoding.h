#ifndef STERRORDECODING_H
#define STERRORDECODING_H 1

//===========================================
// Include files
// from Gaudi
#include "STDecodingBaseAlg.h"


/** @class STErrorDecoding STErrorDecoding.h public/STErrorDecoding.h
 *
 *
 *  @author Mathias Knecht, M Needham
 *  @date   2007-09-11 (2008-06)
 */

class STErrorDecoding : public STDecodingBaseAlg {
public:
  /// Standard constructor
  using STDecodingBaseAlg::STDecodingBaseAlg;

  StatusCode execute   () override;    ///< Algorithm execution

private:

  Gaudi::Property<bool> m_PrintErrorInfo { this, "PrintErrorInfo", false };

};
#endif // STERRORDECODING_H
