#ifndef CONFIGZIPFILEACCESSSVC_H
#define CONFIGZIPFILEACCESSSVC_H 1

// Include files
#include <string>
#include <memory>

#include "ConfigArchiveAccessSvc.h"

/** @class ConfigFileAccessSvc ConfigZipFileAccessSvc.h
 *
 *  functionality:
 *        read/write configure information to zip files
 *
 *  @author Gerhard Raven
 *  @date   2011-04-19
 */

class IArchive;

class ConfigZipFileAccessSvc : public ConfigArchiveAccessSvc {
public:
  using ConfigArchiveAccessSvc::ConfigArchiveAccessSvc;
  StatusCode finalize() override;      ///< Service initialization
  IArchive*  file() const override;
private:
  mutable std::unique_ptr<IArchive>    m_file;
  mutable Gaudi::Property<std::string> m_name { this, "File" };   ///< filename of zip file from which to read configurations
  Gaudi::Property<std::string>         m_mode { this, "Mode", "ReadOnly" };   ///< which flags to specify when opening the zip file
};
#endif // CONFIGZIPFILEACCESSSVC_H
