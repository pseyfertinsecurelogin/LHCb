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
#ifndef COMPONENT_CONDDBDISPATCHERSVC_H
#define COMPONENT_CONDDBDISPATCHERSVC_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "DetCond/ICondDBReader.h"
#include <vector>
#include <map>

template <class TYPE> class SvcFactory;

/** @class CondDBDispatcherSvc CondDBDispatcherSvc.h component/CondDBDispatcherSvc.h
 *
 *
 *
 *  @author Marco Clemencic
 *  @date   2006-07-10
 */
class CondDBDispatcherSvc: public extends1<Service, ICondDBReader> {
public:
  /// Initialize COOL (CondDB) Access Layer Service
  StatusCode initialize() override;
  /// Finalize Service
  StatusCode finalize() override;

  /// Disconnect from the database.
  void disconnect() override;

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

  // --------- ICondDBInfo implementation

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags) const override;

  /// Standard constructor
  CondDBDispatcherSvc( const std::string& name, ISvcLocator* svcloc );

private:

  ICondDBReader *alternativeFor(const std::string &path) const;

  // -------------------- Data Members

  /// Property CondDBDispatcherSvc.MainAccessSvc: the AccessSvc instance to use to retrieve all the
  /// objects for which an alternative is not specified (default to "CondDBAccessSvc").
  std::string m_mainAccessSvcName;

  /// Property CondDBDispatcherSvc.Alternatives: list of alternative Access Services in the form of
  /// "/path/for/alternative":"ServiceType/ServiceName".
  std::map<std::string,std::string> m_alternativesDeclarationMap;

  /// Pointer to the main access service.
  SmartIF<ICondDBReader> m_mainDB;

  /// Container fo the alternatives.
  std::map<std::string,SmartIF<ICondDBReader>> m_alternatives;

  /// Enable/disable direct mapping from the database structure to the transient
  /// store using XML persistency format (enabled by default).
  bool m_xmlDirectMapping;
};
#endif // COMPONENT_CONDDBDISPATCHERSVC_H
