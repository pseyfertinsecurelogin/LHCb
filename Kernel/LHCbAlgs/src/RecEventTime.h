#ifndef RECEVENTTIME_H
#define RECEVENTTIME_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h"            // Interface
#include "GaudiKernel/Time.h"
#include "GaudiKernel/extends.h"

class IEventTimeDecoder;
class IInterface;

/** Simple implementation of IEventTimeDecoder that takes the event
 *  time from LHCb::RecHeader.
 *
 *  @author Marco Clemencic
 *  @date   2010-09-23
 */

class RecEventTime final : public extends1<GaudiTool, IEventTimeDecoder>
{

public:

  /// Standard constructor
  RecEventTime(const std::string& type, const std::string& name,
               const IInterface* parent);

  virtual ~RecEventTime() = default; ///< Destructor

  // --- implementation of IEventTimeDecoder ---
  /// Retrieve the event time from RecHeader (if present).
  /// @return The time of current event.
  Gaudi::Time getTime() const override;

private:

  /// Location of the RecHeader in the transient store.
  std::string m_recHeaderLoc;

};

#endif /* RECEVENTTIME_H */
