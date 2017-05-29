
#ifndef ApplicationVersionFilter_H
#define ApplicationVersionFilter_H 1

// boost
#include <boost/regex.hpp>
#include <string>

// Event
#include "Event/ProcessHeader.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class ApplicationVersionFilter ApplicationVersionFilter.h
 *
 *  Checks the application version from the given ProcessHeader,
 *  comparing it to a given regex pattern to pass/reject the event.
 *
 *  @author Chris Jones
 *  @date   18-04-2013
 */
class ApplicationVersionFilter final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  ApplicationVersionFilter( const std::string& name,
                            ISvcLocator* pSvcLocator );

  virtual ~ApplicationVersionFilter( ) = default; ///< Destructor

  StatusCode execute() override;    ///< Algorithm execution

private:

  std::string m_regex;    ///< String match regex
  std::string m_loc;      ///< DataObject location in TES

};

#endif // DATAOBJECTVERSIONFILTER_H
