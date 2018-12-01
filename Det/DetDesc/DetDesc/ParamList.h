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
#ifndef DETDESC_PARAMLIST_H
#define DETDESC_PARAMLIST_H 1

// Include files

#include "DetDesc/Param.h"

#include "GaudiKernel/Map.h"

/** @class ParamList ParamList.h DetDesc/ParamList.h
 *
 *  Simple implementation of a polymorfic list of parameters identified by
 *  an std::string.
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-02-22
 */
class ParamList final : private GaudiUtils::Map<std::string,BasicParam *> {
private:
  typedef GaudiUtils::Map<std::string,BasicParam *> base_type;

public:
  /// Standard constructor
  ParamList() = default;

  /// Copy constructor
  ParamList(const ParamList &pl);

  /// Destructor
  ~ParamList() { deleteItems(); }

  using base_type::begin;
  using base_type::end;
  using base_type::find;

  /// Add a new parameter to the list (or replace if already there)
  template <class T>
  inline void add(const std::string &key, const T &val) {
    auto i = find(key);
    if ( i != end() ) { // key already used
      i->second->set(val);
    } else {
      insert({key,new Param<T>(val)});
    }
  }

  inline void addBasicParam(const std::string &key, const BasicParam& p) {
    auto i = find(key);
    if ( i != end() ) { // key already used
       // replace with new one
      delete i->second;
      i->second = p.new_copy().release();
    } else {
      insert({key,p.new_copy().release()});
    }
  }

  /// return a vector containing all the stored keys
  virtual std::vector<std::string> getKeys() const;

  /// Copy a list into this one (deleting this one)
  ParamList& operator= (const ParamList &pl);

  /// Merge two lists (overwriting objects with the same name)
  ParamList& operator+= (const ParamList &pl);

  /// Remove all elements from the list, deleting the objects
  void clear();

private:
  void deleteItems();

};
#endif // DETDESC_PARAMLIST_H
