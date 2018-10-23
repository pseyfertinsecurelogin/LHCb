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
#ifndef COMPONENT_CONDDBLAYERINGSVC_H
#define COMPONENT_CONDDBLAYERINGSVC_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "DetCond/ICondDBReader.h"
#include <vector>

template <class TYPE> class SvcFactory;

/** @class CondDBLayeringSvc CondDBLayeringSvc.h component/CondDBLayeringSvc.h
 *
 *
 *  @author Marco CLEMENCIC
 *  @date   2006-07-14
 */
class CondDBLayeringSvc: public extends1<Service, ICondDBReader> {
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
  CondDBLayeringSvc( const std::string& name, ISvcLocator* svcloc );

private:

  // -------------------- Data Members

  /// Property CondDBLayeringSvc.Layers: list of Access Service layers.
  /// They will be searched from the first to the last.
  std::vector<std::string> m_layersNames;

  /// Container fo the alternatives.
  std::vector<SmartIF<ICondDBReader>> m_layers;

  /// Enable/disable direct mapping from the database structure to the transient
  /// store using XML persistency format (enabled by default).
  bool m_xmlDirectMapping;

  /// Internal implementation helper to generalize the channel type.
  template <typename Channel>
  IOVList i_getIOVs (const std::string &path, const IOV &iov, const Channel &channel);

};
#endif // COMPONENT_CONDDBLAYERINGSVC_H
