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
#ifndef COMPONENT_CONDDBTIMESWITCHSVC_H
#define COMPONENT_CONDDBTIMESWITCHSVC_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "GaudiKernel/Map.h"
#include "DetCond/ICondDBReader.h"
#include <vector>
#include <map>

template <class TYPE> class SvcFactory;
class IDetDataSvc;

/** @class CondDBTimeSwitchSvc CondDBTimeSwitchSvc.h component/CondDBTimeSwitchSvc.h
 *
 *
 *
 *  @author Marco Clemencic
 *  @date   2006-07-10
 */
class CondDBTimeSwitchSvc: public extends<Service, ICondDBReader> {
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
  CondDBTimeSwitchSvc( const std::string& name, ISvcLocator* svcloc );

private:

  /// Internal class to record the readers
  struct ReaderInfo final {
    /// CondDBReader instance name ("type/name")
    std::string name;
    /// Boundaries of the Interval Of Validity
    Gaudi::Time since, until;
    /// Default Constructor
    ReaderInfo(const std::string &_n,
        Gaudi::Time _s = Gaudi::Time::epoch(),
        Gaudi::Time _u = Gaudi::Time::max()):
      name(_n), since(_s), until(_u)
    {}
    /// Shortcut to check the validity of the reader
    bool isValidAt(const Gaudi::Time &when) const {
      return since <= when && until > when;
    }
    /// Shortcut to retrieve the pointer to the reader service.
    ICondDBReader *reader(ISvcLocator *svcloc) const {
      if (!m_reader) {
        if (!svcloc)
          throw GaudiException("ServiceLocator pointer is NULL",
              "CondDBTimeSwitchSvc::ReaderInfo::get",StatusCode::FAILURE);
        m_reader = svcloc->service(name,true);
        if (!m_reader)
          throw GaudiException("Cannot get ICondDBReader '"+name+"'",
              "CondDBTimeSwitchSvc::ReaderInfo::get",StatusCode::FAILURE);
      }
      return m_reader.get();
    }
    /// Shortcut to restrict and IOV to the boundaries defined for the reader
    void cutIOV(Gaudi::Time &_since, Gaudi::Time &_until) const {
      if ( since > _since ) _since = since;
      if ( until < _until ) _until = until;
    }
    /// convert to a string
    std::string toString() const;
    /// tell if the reader has already been instantiated
    bool loaded() const { return m_reader; }
  private:
    /// Pointer to the CondDBReader instance
    mutable SmartIF<ICondDBReader> m_reader;
  };

  /// Get the the CondDBReader valid for a given point in time.
  /// Returns 0 if no service is available.
  /// The boolean flag is used to avoid messages (during initialization).
  ReaderInfo *readerFor(const Gaudi::Time &when, bool quiet = false);

  /// Get the the CondDBReader valid for a given point in time.
  /// Returns 0 if no service is available
  ReaderInfo *currentReader();

  /// Get current event time from the detector data svc.
  Gaudi::Time getTime();

  // -------------------- Data Members
  typedef std::vector<std::string> ReadersDeclatationsType;
  typedef GaudiUtils::Map<Gaudi::Time,ReaderInfo> ReadersType;

  /// Property CondDBTimeSwitchSvc.Readers: list of ICondDBReaders to be used
  /// for given intervals of validity. The format is "'Reader': (since, until)",
  /// where since and until are doubles defining the time in standard units.
  ReadersDeclatationsType m_readersDeclatations;

  /// Container for the alternatives. The ReaderInfo objects are indexed by
  /// "until" to allow efficient search with map::upper_bound.
  ReadersType m_readers;

  /// Pointer used to cache the latest requested reader.
  /// It allows to avoid the search.
  ReaderInfo *m_latestReaderRequested = nullptr;

  /// Pointer to the detector data service, used to get the event time in
  /// the methods that do not require it as argument.
  /// It used only if there was not a previous request with the time.
  SmartIF<IDetDataSvc> m_dds;

  /// Enable/disable direct mapping from the database structure to the transient
  /// store using XML persistency format (enabled by default).
  bool m_xmlDirectMapping;

  /// Internal implementation helper to generalize the channel type.
  template <typename Channel>
  IOVList i_getIOVs (const std::string &path, const IOV &iov, const Channel &channel);
};
#endif // COMPONENT_CONDDBDISPATCHERSVC_H
