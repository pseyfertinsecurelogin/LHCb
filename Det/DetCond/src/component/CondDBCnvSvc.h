#ifndef DETCOND_CONDDBCNVSVC_H
#define DETCOND_CONDDBCNVSVC_H 1

#include <CoolKernel/ChannelId.h>
#include <string>
#include <vector>

#include "DetCond/ICondDBReader.h"
#include "GaudiKernel/ClassID.h"
/// Include files
#include "GaudiKernel/ConversionSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/extends.h"
#include "Kernel/CondDBNameTagPair.h"

class IConverter;
class IDetDataSvc;
class IOpaqueAddress;
class ISvcLocator;
/// Forward and external declarations
template <class TYPE> class SvcFactory;

///---------------------------------------------------------------------------
/** @class CondDBCnvSvc CondDBCnvSvc.h

    A conversion service for CERN-IT COOL (ex. CondDB) persistency.
    Allows to create and update condition data objects (i.e. DataObjects
    implementing IValidity).

    @author Marco Clemencic
    @date November 2004
*///--------------------------------------------------------------------------

class CondDBCnvSvc : public extends1<ConversionSvc, ICondDBReader> {

  /// Only factories can access protected constructors
  friend class SvcFactory<CondDBCnvSvc>;

protected:

  /// Constructor
  CondDBCnvSvc( const std::string& name, ISvcLocator* svc );

public:

  // Overloaded from ConversionSvc

  /// Initialize the service
  StatusCode initialize() override;

  /// Finalize the service
  StatusCode finalize() override;

  using ConversionSvc::createAddress;
  /// Create an address using explicit arguments to identify a single object.
  StatusCode createAddress (long svc_type,
                             const CLID& clid,
                             const std::string* par,
                             const unsigned long* ip,
                             IOpaqueAddress*& refpAddress ) override;

public:

  /// Retrieve converter from list
  IConverter* converter(const CLID& clid) override;

  // --------- ICondDBReader implementation

  /// Try to retrieve an object from the Condition DataBase. If path points to a FolderSet,
  /// channel and when are ignored and data is set ot NULL.
  StatusCode getObject (const std::string &path, const Gaudi::Time &when,
                        DataPtr &data,
                        std::string &descr, Gaudi::Time &since, Gaudi::Time &until, cool::ChannelId channel = 0) override;

  /// Try to retrieve an object from the Condition DataBase. If path points to a FolderSet,
  /// channel and when are ignored and data is set ot NULL.
  StatusCode getObject (const std::string &path, const Gaudi::Time &when,
                        DataPtr &data,
                        std::string &descr, Gaudi::Time &since, Gaudi::Time &until, const std::string &channel) override;

  /// @{
  /// @see ICondDBReader::getIOVs
  IOVList getIOVs (const std::string &path, const IOV &iov, cool::ChannelId channel = 0) override;
  IOVList getIOVs (const std::string &path, const IOV &iov, const std::string &channel) override;
  /// @}

  /// Retrieve the names of the children nodes of a FolderSet.
  StatusCode getChildNodes (const std::string &path, std::vector<std::string> &node_names) override;

  /// Retrieve the names of the children nodes of a FolderSet divided in folders and foldersets.
  StatusCode getChildNodes (const std::string &path,
                            std::vector<std::string> &folders,
                            std::vector<std::string> &foldersets) override;

  /// Tells if the path is available in the database.
  bool exists(const std::string &path) override;

  /// Tells if the path (if it exists) is a folder.
  bool isFolder(const std::string &path) override;

  /// Tells if the path (if it exists) is a folderset.
  bool isFolderSet(const std::string &path) override;

  /// Disconnect from the database.
  void disconnect() override;

  // --------- ICondDBInfo implementation

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags) const override;

private:

  /// List of all the names of the known databases. It is filled via the option
  /// CondDBCnvSvc.CondDBReader. If none is given, "CondDBAccessSvc" is used.
  std::string    m_dbReaderName;

  /// Handles to the database Access services
  SmartIF<ICondDBReader> m_dbReader;

};

#endif    // DETCOND_CONDITIONSDBCNVSVC_H


