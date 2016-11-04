#ifndef STERRORDECODING_H
#define STERRORDECODING_H 1

//===========================================
// Include files
// from Gaudi
#include "GaudiAlg/Consumer.h"
#include "STDecodingBaseAlg.h"


/** @class STErrorDecoding STErrorDecoding.h public/STErrorDecoding.h
 *
 *
 *  @author Mathias Knecht, M Needham, S Ponce
 *  @date   2007-09-11 (2008-06)
 */

class STErrorDecoding : public Gaudi::Functional::Consumer<void(const LHCb::RawEvent&),
                                                           Gaudi::Functional::Traits::BaseClass_t<STDecodingBaseAlg>> {
public:
  /// Standard constructor
  STErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm execution
  void operator()(const LHCb::RawEvent&) const override;

private:

  bool m_PrintErrorInfo;

};
#endif // STERRORDECODING_H
