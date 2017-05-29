#ifndef TIMEDECODERLIST_H
#define TIMEDECODERLIST_H 1

#include <list>
#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h"            // Interface
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/extends.h"

class IEventTimeDecoder;
class IInterface;

/** Simple implementation of IEventTimeDecoder forwarding the calls to a list of
 *  other implementations.
 *
 *  @author Marco Clemencic
 *  @date   2010-09-23
 */

class TimeDecoderList final : public extends1<GaudiTool, IEventTimeDecoder>
{

public:

  /// Standard constructor
  TimeDecoderList(const std::string& type,
                  const std::string& name,
                  const IInterface* parent);

  virtual ~TimeDecoderList() = default; ///< Destructor

  /// Initialize the tool
  StatusCode initialize() override;

  // --- implementation of IEventTimeDecoder ---
  /// Loop over all the used decoders and return the first non-zero event time.
  /// @return The time of current event.
  Gaudi::Time getTime() const override;

private:

  /// Type of the list of decoder names.
  typedef std::vector<std::string> DecoderNamesType;

  /// Type of the list of decoders.
  typedef std::list<IEventTimeDecoder*> DecodersType;

  /// List of IEventTimeDecoder instance names.
  DecoderNamesType m_decoderNames;

  /// List of IEventTimeDecoder instances
  DecodersType m_decoders;

};

#endif /* TIMEDECODERLIST_H */
