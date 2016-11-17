#ifndef HLTSELREPORTSWRITER_H 
#define HLTSELREPORTSWRITER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/HltObjectSummary.h"


#include "Kernel/IANNSvc.h"


/** @class HltSelReportsWriter HltSelReportsWriter.h
 *  
 *
 *  @author Tomasz Skwarnicki
 *  @date   2008-07-25
 *
 *  Algorithm to convert HltSelReports and HltObjectSummarys containers on TES to HltSelCandidates Raw Bank
 *
 */
class HltSelReportsWriter : public GaudiAlgorithm {
public: 
  /// Standard constructor
  HltSelReportsWriter( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

  enum HeaderIDs { kVersionNumber=10 };


  enum SourceIDs { kSourceID_Dummy=0,
		   kSourceID_Hlt=kSourceID_Dummy,
		   kSourceID_Hlt1=1,
		   kSourceID_Hlt2=2,
		   kSourceID_Max=7,
		   kSourceID_BitShift=13,
		   kSourceID_MinorMask=0x1FFF,
		   kSourceID_MajorMask=0xE000
  };
private:

  /// location of input H
  StringProperty m_inputHltSelReportsLocation;

  /// location of output
  StringProperty m_outputRawEventLocation;

  /// SourceID to insert in the bank header (0-7)
  IntegerProperty m_sourceID;
  
  /// HltANNSvc for making selection names to int selection ID
  SmartIF<IANNSvc> m_hltANNSvc;
};


#endif // HLTSELREPORTSWRITER_H
