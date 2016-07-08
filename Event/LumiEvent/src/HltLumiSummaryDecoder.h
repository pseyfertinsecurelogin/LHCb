#ifndef HLTLUMISUMMARYDECODER_H
#define HLTLUMISUMMARYDECODER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/TransformAlgorithm.h"
#include "Event/HltLumiSummary.h"
// std
#include <string>

/** @class HltLumiSummaryDecoder HltLumiSummaryDecoder.h
 *
 *  Decodes the LumiSummary. 
 *
 *  @author Jaap Panman
 *
 *  @date   2008-08-01
 */

// HenryIII Changed to use Transform Algorithm

class HltLumiSummaryDecoder : public TransformAlgorithm<LHCb::HltLumiSummary(const LHCb::RawEvent&)> {
public: 
  /// Standard constructor
  HltLumiSummaryDecoder(  const std::string& name, ISvcLocator* pSvcLocator );
  
  /// Destructor
  virtual ~HltLumiSummaryDecoder( ) ; ///< Destructor
  
  virtual StatusCode initialize();    ///< Algorithm initialization
  LHCb::HltLumiSummary operator() (const LHCb::RawEvent& event) const override;    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization
  
protected:
  
private:
  // data
  std::string m_OutputContainerName;

  // Statistics  
  mutable double m_totDataSize;
  mutable int m_nbEvents;

};

//////////////////////////////////////////////////////////////////////////////////////////
#endif // HLTLUMISUMMARYDECODER_H
