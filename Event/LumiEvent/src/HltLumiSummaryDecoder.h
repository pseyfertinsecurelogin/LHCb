#ifndef HLTLUMISUMMARYDECODER_H
#define HLTLUMISUMMARYDECODER_H 1

// Include files
// from Gaudi
#include "Event/HltLumiSummary.h"
#include "GaudiAlg/Transformer.h"
// std
#include <string>
#include <atomic>

/** @class HltLumiSummaryDecoder HltLumiSummaryDecoder.h
 *
 *  Decodes the LumiSummary.
 *
 *  @author Jaap Panman
 *
 *  @date   2008-08-01
 */

// HenryIII Changed to use Transform Algorithm

class HltLumiSummaryDecoder : public Gaudi::Functional::Transformer<
                                  LHCb::HltLumiSummary(const LHCb::RawEvent&)
                              >
{
public:
  /// Standard constructor
  HltLumiSummaryDecoder(  const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  LHCb::HltLumiSummary operator() (const LHCb::RawEvent& event) const override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  // Statistics, mutable to allow statistics to be kept
  mutable std::atomic<double> m_totDataSize = {0};
  mutable std::atomic<int> m_nbEvents = {0};

};

//////////////////////////////////////////////////////////////////////////////////////////
#endif // HLTLUMISUMMARYDECODER_H
