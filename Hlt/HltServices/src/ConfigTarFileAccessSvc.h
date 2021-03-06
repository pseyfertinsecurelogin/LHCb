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
#ifndef CONFIGTARFILEACCESSSVC_H
#define CONFIGTARFILEACCESSSVC_H 1

// Include files
#include "ConfigArchiveAccessSvc.h"
#include <memory>
#include <string>

/** @class ConfigFileAccessSvc ConfigFileAccessSvc.h
 *
 *  functionality:
 *        read/write configure information to files
 *
 *  @author Gerhard Raven
 *  @date   2009-06-04
 */
class IArchive;

class ConfigTarFileAccessSvc : public ConfigArchiveAccessSvc {
public:
  using ConfigArchiveAccessSvc::ConfigArchiveAccessSvc;

  StatusCode finalize() override; ///< Service initialization
  IArchive*  file() const override;

private:
  mutable std::unique_ptr<IArchive>    m_file;
  mutable Gaudi::Property<std::string> m_name{this, "File"}; ///< filename of tar file from which to read configurations
  Gaudi::Property<std::string> m_mode{this, "Mode", "ReadOnly"}; ///< which flags to specify when opening the tar file
  Gaudi::Property<bool>        m_compress{this, "CompressOnWrite",
                                   true}; ///< do we want to transparently compress items on write?
};
#endif // CONFIGTARFILEACCESSSVC_H
