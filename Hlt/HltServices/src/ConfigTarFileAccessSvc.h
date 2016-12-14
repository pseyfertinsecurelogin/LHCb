#ifndef CONFIGTARFILEACCESSSVC_H
#define CONFIGTARFILEACCESSSVC_H 1

// Include files
#include <string>
#include <memory>
#include "ConfigArchiveAccessSvc.h"

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

  StatusCode finalize() override;      ///< Service initialization
  IArchive*  file() const override;

private:

  mutable std::unique_ptr<IArchive>    m_file;
  mutable Gaudi::Property<std::string> m_name { this, "File" };   ///< filename of tar file from which to read configurations
  Gaudi::Property<std::string>         m_mode { this, "Mode", "ReadOnly" };   ///< which flags to specify when opening the tar file
  Gaudi::Property<bool>                m_compress { this, "CompressOnWrite", true }; ///< do we want to transparently compress items on write?
};
#endif // CONFIGTARFILEACCESSSVC_H
