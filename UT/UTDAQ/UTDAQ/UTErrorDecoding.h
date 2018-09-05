#ifndef UTERRORDECODING_H
#define UTERRORDECODING_H 1

//===========================================
// Include files
// from Gaudi
#include "GaudiAlg/Consumer.h"
#include "UTDecodingBaseAlg.h"


/** @class UTErrorDecoding UTErrorDecoding.h public/UTErrorDecoding.h
 *
 *
 *  @author A Beiter (based on code by Mathias Knecht, M Needham, S Ponce)
 *  @date   2018-09-04
 */

class UTErrorDecoding : public Gaudi::Functional::Consumer<void(const LHCb::RawEvent&),
                                                           Gaudi::Functional::Traits::BaseClass_t<UTDecodingBaseAlg>> {
public:
  /// Standard constructor
  UTErrorDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm execution
  void operator()(const LHCb::RawEvent&) const override;

private:

  Gaudi::Property<bool> m_PrintErrorInfo { this, "PrintErrorInfo", false };

};
#endif // UTERRORDECODING_H
