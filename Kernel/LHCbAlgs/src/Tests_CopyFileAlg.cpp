#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <functional>
#include <string>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "Gaudi/PluginService.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

#define BOOST_FILESYSTEM_NO_DEPRECATED

using namespace boost::filesystem;

namespace LHCbAlgsTests
{

  /// Test algorithm that copies a file when executed.
  class CopyFileAlg final : public GaudiAlgorithm
  {

  public:

    CopyFileAlg(const std::string& name, ISvcLocator* pSvcLocator)
      : GaudiAlgorithm(name, pSvcLocator)
    {
      declareProperty("Source", m_source="");
      declareProperty("Destination", m_destination="");
    }

    StatusCode execute() override
    {
      path src = m_source;
      path dst = m_destination;

      if (msgLevel(MSG::DEBUG))
        debug() << "Copying " << m_source << " -> " << m_destination << endmsg;

      copy_file(src, dst, copy_option::overwrite_if_exists);

      return StatusCode::SUCCESS;
    }

  private:

    std::string m_source, m_destination;

  };

  DECLARE_COMPONENT(CopyFileAlg)
}
