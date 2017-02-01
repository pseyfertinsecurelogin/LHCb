// $Id: EventCountHisto.h,v 1.1 2009-02-11 17:12:33 rlambert Exp $
#ifndef EVENTCOUNTHISTO_H
#define EVENTCOUNTHISTO_H 1

// Include files
#include "GaudiAlg/GaudiHistoAlg.h"


/** @class EventCountHisto EventCountHisto.h
 *
 *
 *  @author Robert Lambert
 *  @date   2007-01-08
 */
class EventCountHisto final : public GaudiHistoAlg
{

public:

  /// Standard constructor
  EventCountHisto( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~EventCountHisto( ) = default; ///< Destructor

  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  unsigned long long m_nEvents{0}; // number of events processed

};

#endif // EVENTCOUNTHISTO_H
