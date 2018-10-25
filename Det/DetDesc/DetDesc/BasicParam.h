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
#ifndef DETDESC_BASICPARAM_H 
#define DETDESC_BASICPARAM_H 1

// Include files
#include <typeinfo>
#include <string>
#include <memory>

/** @class BasicParam BasicParam.h DetDesc/BasicParam.h
 *  
 *  Simple class used to provide common handle for generic parameters.
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-02-22
 */
class BasicParam {
public:

  /// Virtual destructor, just to be safe.
  virtual ~BasicParam() = default;
  
  /// Generic getter. When accessing the datum of a parameter through the base class
  /// you should specify the exact type (no automatic conversion implemented).
  template <class T>
  inline T &get() {
    if (type() != typeid(T)) { throw std::bad_cast(); }
    return *static_cast<T*>(_get_ptr());
  }
  /// Generic getter (const version). see above
  template <class T>
  inline const T &get() const {
    if (type() != typeid(T)) { throw std::bad_cast(); }
    return *static_cast<T*>(_get_ptr());
  }
  
  /// Generic setter. Same cosideration as for the getters.
  template <class T>
  inline void set(const T &val) {
    if (type() != typeid(T)) { throw std::bad_cast(); }
    *static_cast<T*>(_get_ptr()) = val;
  }

  ///  String representation for printout
  virtual std::string toStr() const = 0;
  
  ///  String representation for XML format
  virtual std::string toXMLStr(const std::string &name,
                               const std::string& comment = "",
                               int precision = 16) const = 0;

  /// TypeId of the datum
  virtual const std::type_info &type() const = 0;

  /// Used for generic copy. You can copy type-safely a parameter via the base class. 
  /// (Needed by ParamList)
  virtual std::unique_ptr<BasicParam> new_copy() const = 0;

private:
  /// Handle to access the real datum
  virtual void *_get_ptr() = 0;
  /// Handle to access the real datum
  virtual const void *_get_ptr() const = 0;

};
#endif // DETDESC_BASICPARAM_H
