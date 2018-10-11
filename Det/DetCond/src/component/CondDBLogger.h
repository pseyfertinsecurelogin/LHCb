/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef COMPONENT_CONDDBLOGGER_H
#define COMPONENT_CONDDBLOGGER_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "DetCond/ICondDBReader.h"

template <class TYPE> class SvcFactory;

/** @class CondDBLogger CondDBLogger.h component/CondDBLogger.h
 *
 *  Logger of acesses to CondDB.
 *
 *  CondDBLogger is a simple class that allow to store in a file all the
 *  requests made to a ICondDBReader instance.  It has to be used as a front-end
 *  to the instance we want to monitor.
 *
 *  Given the following option snippet
 *  @code
 *  MyCondDBUser.Reader = "ACondDBReader";
 *  @endcode
 *  the CondDBLogger can be enabled with
 *  @code
 *  CondDBLogger.LoggedReader = "ACondDBReader";
 *  MyCondDBUser.Reader = "CondDBLogger";
 *  @endcode
 *  or in python options
 *  @code
 *  user = MyCondDBUser()
 *  user.Reader = CondDBLogger(LoggedReader = user.Reader)
 *  @endcode
 *
 *  The format of the log file is very simple.  Each line starts with an
 *  operation code then the time of the operation in ns (as returned by
 *  Gaudi::Time::ns()).  The rest of the line depend on the operation:
 *  - "INI:"
 *    - Initialization of the logger
 *  - "TAG:"
 *    - Request of the used tag
 *  - "GCH:"
 *    - Retrieve the child nodes of a folderset.
 *  - "GET:"
 *    - Request of an object from the database, the format is<br>
 *      &lt;path&gt; &lt;channel&nbsp;id&gt; &lt;path&gt; &lt;evt.time&gt; &lt;status&gt;
 *  - "GCN:"
 *    - Request of an object from the database using the channel name, the format is<br>
 *      &lt;path&gt; &lt;channel&nbsp;name&gt; &lt;path&gt; &lt;evt.time&gt; &lt;status&gt;
 *  - "FIN:"
 *    - Finalization of the logger
 *  - "IOV:"
 *    - Request list of IOVs (using numeric channel)
 *  - "ICN:"
 *    - Request list of IOVs (using channel name)
 *
 *  @param LoggedReader
 *         Fully qualified name of the ICondDBReader to which the calls have to
 *         be forwarded.
 *  @param LogFile
 *         Path to the log file (it is overwritten if it exists).  If not
 *         specified or set to empty, the file name is set from the name of the
 *         instance plus '.log'.
 *
 *  @author Marco CLEMENCIC
 *  @date   2008-01-24
 */
class CondDBLogger: public extends1<Service, ICondDBReader> {
public:
  /// Initialize COOL (CondDB) Access Layer Service
  StatusCode initialize() override;
  /// Finalize Service
  StatusCode finalize() override;

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

  /// Standard constructor
  CondDBLogger( const std::string& name, ISvcLocator* svcloc );

  ~CondDBLogger( ); ///< Destructor

private:

  // -------------------- Data Members

  /// Pointer to the CondDBReader whose activity has to be logged.
  ICondDBReader *m_loggedReader;

  /// Name of the CondDBReader whose activity has to be logged.
  std::string m_loggedReaderName;

  /// Path to the file that will contain the log.
  std::unique_ptr<std::ostream> m_logFile;

  /// Path to the file that will contain the log.
  std::string m_logFileName;

};
#endif // COMPONENT_CONDDBLAYERINGSVC_H
