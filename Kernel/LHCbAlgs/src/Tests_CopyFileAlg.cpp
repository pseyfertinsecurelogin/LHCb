/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "GaudiAlg/GaudiAlgorithm.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>
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
