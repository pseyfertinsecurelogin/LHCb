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
#ifndef COMPONENT_CONDDBCACHE_H
#define COMPONENT_CONDDBCACHE_H 1

// Include files
#include <string>
#include <vector>
#include <list>
#include <numeric>

#include "GaudiKernel/MsgStream.h"

#include "GaudiKernel/HashMap.h"

#include "CoolKernel/types.h"
#include "CoolKernel/pointers.h"
#include "CoolKernel/ValidityKey.h"
#include "CoolKernel/IObject.h"
#include "CoolKernel/IFolder.h"
#include "CoolKernel/IRecord.h"
#include "CoolKernel/Record.h"
#include "CoolKernel/IRecordSpecification.h"
#include "CoolKernel/RecordSpecification.h"

#include "DetCond/ICondDBReader.h"

#include <memory>

/** @class CondDBCache CondDBCache.h component/CondDBCache.h
 *
 *  Class used to manage in memory conditions.
 *
 *  @author Marco Clemencic
 *  @date   2005-06-13
 */
class CondDBCache final {

public:

  typedef std::pair<cool::ValidityKey,cool::ValidityKey> IOVType;

  //--------------------------------------------------------------------------------
  /// Standard constructor
  CondDBCache(const MsgStream& log, size_t highLevel = 100, size_t lowLevel = 10);

   ~CondDBCache( ); ///< Destructor

  /// Add a new data object to the cache.
  /// \warning {no check performed}
  bool insert(const cool::IFolderPtr &folder,const cool::IObject &obj, const cool::ChannelId &channel = 0);

  /// Shortcut for the regular implementations (for backward compatibility).
  inline bool insert(const cool::IFolderPtr &folder,const cool::IObjectPtr &obj, const cool::ChannelId &channel = 0) {
    return insert(folder, *obj.get(), channel);
  }

  bool addFolder(const std::string &path, const std::string &descr, const cool::IRecordSpecification& spec);
  bool addFolder(const std::string &path, const std::string &descr, const cool::IRecordSpecification& spec,
                 const std::map<cool::ChannelId,std::string>& ch_names);
  bool addFolderSet(const std::string &path, const std::string &descr);
  bool addObject(const std::string &path, const cool::ValidityKey &since, const cool::ValidityKey &until,
                 const cool::IRecord& rec, const cool::ChannelId &channel, IOVType *iov_before = NULL);
  /// (version kept for backward compatibility)
  inline bool addObject(const std::string &path, const cool::ValidityKey &since, const cool::ValidityKey &until,
                        const cool::IRecord& rec, IOVType *iov_before = NULL)
  {
    return addObject(path,since,until,rec,0,iov_before);
  }


  /// Search an entry in the cache and returns the data string or an empty string if no object is found.
  bool get(const std::string &path, const cool::ValidityKey &when,
           const cool::ChannelId &channel,
           cool::ValidityKey &since, cool::ValidityKey &until,
           std::string &descr, ICondDBReader::DataPtr &payload);

  /// Search an entry in the cache and returns the data string or an empty string if no object is found.
  /// (version kept for backward compatibility)
  inline bool get(const std::string &path, const cool::ValidityKey &when,
                  cool::ValidityKey &since, cool::ValidityKey &until,
                  std::string &descr, ICondDBReader::DataPtr &payload) {
    return get(path,when,0,since,until,descr,payload);
  }

  /// Find the value of the channel id for the given channel name in a folder
  /// (if present in the cache).
  /// Returns true if the channel name in the folder was found
  bool getChannelId(const std::string &path,const std::string &name,
                    cool::ChannelId &channel) const;

  void getSubNodes(const std::string &path, std::vector<std::string> &node_names);

  void getSubNodes(const std::string &path, std::vector<std::string> &folders, std::vector<std::string> &foldersets);

  /// Remove all entries from the cache;
  inline void clear() {m_cache.clear();}

  /// Get the number of items cached.
  inline size_t size() const;

  inline void setHighLevel(size_t lvl) { m_highLvl = lvl; }
  inline void setLowLevel(size_t lvl) { m_lowLvl = lvl; }
  inline size_t highLevel() const { return m_highLvl; }
  inline size_t lowLevel() const { return m_lowLvl; }

  inline size_t level() const { return m_level; }

  void clean_up();

  /// Check if the given path is present in the cache.
  inline bool hasPath(const std::string &path) const { return m_cache.count(path) != 0; }

  /// Check if the path is a folderset.
  inline bool isFolderSet(const std::string &path) const {
    return hasPath(path) && (m_cache.find(path)->second.spec.get() == 0);
  }

  /// Check if the path is a folderset.
  inline bool isFolder(const std::string &path) const {
    return hasPath(path) && (m_cache.find(path)->second.spec.get() != 0);
  }

  /// Check if the given path,time pair is present in the cache.
  bool hasTime(const std::string &path, const cool::ValidityKey &when, const cool::ChannelId &channel = 0) const;

  /// Return the list of IOVs known for the given path, IOV, channel.
  /// @see ICondDBReader::getIOVs
  ICondDBReader::IOVList getIOVs(const std::string &path, const ICondDBReader::IOV &iov, cool::ChannelId channel = 0);

  void dump();

  /// Set the flag to enable the check that the inserted IOVs are not compatible with the latest
  /// requested time (needed to avoid that the cache is modified for the current event).
  /// @return previous value
  bool setIOVCheck(bool enable)
  {
    bool old = m_checkLastReqTime;
    m_checkLastReqTime = enable;
    return old;
  }

  /// Tell if the check on inserted IOVs is enabled.
  bool IOVCheck() { return m_checkLastReqTime; }

  /// Getter for the data member m_silentConflicts.
  bool silentConflicts() const { return m_silentConflicts; }

  /// Getter for the data member m_silentConflicts.
  void setSilentConflicts(bool value) { m_silentConflicts = value; }

private:

  struct CondFolder;
  struct CondItem;

  typedef std::string FolderIdType;
  //typedef std::vector<CondItem> ItemListType;
  typedef std::list<CondItem> ItemListType;
  //  typedef std::map<FolderIdType,CondFolder> FolderListType;
  typedef GaudiUtils::HashMap<FolderIdType,CondFolder> StorageType;

  /// Internal class used to record IOV+data pairs
  struct CondItem final {
    /// Constructor.
    CondItem(CondFolder *myFolder, const cool::IObject &obj):
      folder(myFolder),iov(obj.since(),obj.until()),
      data(new cool::Record(obj.payload())),score(1.0) {}
    CondItem(CondFolder *myFolder, const cool::ValidityKey &since, const cool::ValidityKey &until,
             const cool::IRecord &rec):
      folder(myFolder),iov(since,until), data(new cool::Record(rec)),score(1.0) {}
    CondFolder *folder;
    IOVType iov;
    ICondDBReader::DataPtr data;
    float score;
    /// Check if the CondItem is valid at the given time.
    inline bool valid(const cool::ValidityKey &when) const {
      return iov.first <= when && when < iov.second;
    }
  };

  /// Internal class used to keep the items common to a given path.
  struct CondFolder final {

    typedef GaudiUtils::Map<cool::ChannelId,ItemListType> StorageType;
    typedef GaudiUtils::HashMap<std::string,cool::ChannelId> ChannelNamesMapType;

    CondFolder(const cool::IFolderPtr &fld):
      description(fld->description()),
      spec(new cool::RecordSpecification(fld->payloadSpecification())),
      sticky(false) {}
    CondFolder(const std::string &descr, const cool::IRecordSpecification& new_spec):
      description(descr),spec(new cool::RecordSpecification(new_spec)),sticky(true) {}
    // for a folderset (FolderSets are identified by missing spec)
    CondFolder(const std::string &descr):
      description(descr),sticky(true) {}
    std::string description;
    std::shared_ptr<cool::IRecordSpecification> spec;
    StorageType items;
    ChannelNamesMapType channelNames;
    bool sticky;
    /// Search for the first item in the storage valid at the given time.
    inline ItemListType::iterator find(const cool::ValidityKey &when, const cool::ChannelId &channel = 0) {
      ItemListType &lst = items[channel];
      return std::find_if( lst.begin(), lst.end(),
                           [&](ItemListType::const_reference i)
                           { return i.valid(when); } );
    }
    /// Const version of the search method.
    inline ItemListType::const_iterator find(const cool::ValidityKey &when, const cool::ChannelId &channel = 0) const {
      const ItemListType &lst = (*const_cast<StorageType *>(&items))[channel];
      return std::find_if( lst.begin(), lst.end(),
                           [&](ItemListType::const_reference i)
                           { return i.valid(when); } );
    }
    inline ItemListType::iterator conflict(const cool::ValidityKey &since, const cool::ValidityKey &until,
                                           const cool::ChannelId &channel = 0) {
      ItemListType &lst = items[channel];
      return std::find_if( lst.begin(), lst.end(),
                           [&](ItemListType::const_reference i) {
        // Given two IOVs a and b, they conflict if the intersection is not empty:
        //   max(a.s,b.s) < min(a.u,b.u)
                            return std::max(i.iov.first, since) < std::min(i.iov.second, until);
      } );
    }
    inline ItemListType::const_iterator conflict(const cool::ValidityKey &since, const cool::ValidityKey &until,
                                                 const cool::ChannelId &channel = 0) const {
      const ItemListType &lst = (*const_cast<StorageType *>(&items))[channel];
      return std::find_if( lst.begin(), lst.end(),
                           [&](ItemListType::const_reference i) {
        // Given two IOVs a and b, they conflict if the intersection is not empty:
        //   max(a.s,b.s) < min(a.u,b.u)
                            return std::max(i.iov.first, since) < std::min(i.iov.second, until);
      } );
    }
    inline ItemListType::iterator end(const cool::ChannelId &channel = 0) {
      return items[channel].end();
    }
    inline ItemListType::const_iterator end(const cool::ChannelId &channel = 0) const {
      return (*const_cast<StorageType *>(&items))[channel].end();
    }
    inline void erase (const cool::ValidityKey &when, const cool::ChannelId &channel = 0) {
      items[channel].erase(find(when,channel));
    }
    inline bool empty() const {
      return std::all_of( items.begin(), items.end(),
                          [](StorageType::const_reference ch)
                          { return ch.second.empty(); } );
    }

  };

  /// Actual storage
  StorageType m_cache;

  size_t m_highLvl;
  size_t m_lowLvl;
  size_t m_level;

  MsgStream m_log;

  cool::ValidityKey m_lastRequestedTime;
  bool m_checkLastReqTime;

  // Do not print warning messages in case of conflicts during the insertion
  bool m_silentConflicts;
};

inline size_t CondDBCache::size() const {
  return std::accumulate( m_cache.begin(), m_cache.end(), (size_t)0,
                          [](size_t count, StorageType::const_reference folder) {
    return std::accumulate( folder.second.items.begin(), folder.second.items.end(),
                            count, [](size_t c, CondFolder::StorageType::const_reference ch)
                            { return c+ch.second.size(); } ); } );
}

#endif // COMPONENT_CONDDBCACHE_H
