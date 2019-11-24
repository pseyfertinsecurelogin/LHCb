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
#ifndef DETDESC_PARAM_H
#define DETDESC_PARAM_H 1

// Include files
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "DetDesc/BasicParam.h"
#include "GaudiKernel/GaudiException.h"

/** @class Param Param.h DetDesc/Param.h
 *
 *  Class implementing a polymorfic parameter using templates
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-02-22
 */
template <class T>
class Param : public BasicParam {
public:
  typedef T value_type;

  /// Standard constructor
  Param() = default;

  /// Constructor by value
  Param( value_type val ) : m_val( std::move( val ) ) {}

  /// Setter (with automatic conversion)
  template <class T1>
  void set( T1 val ) {
    m_val = std::move( val );
  }

  /// Getter
  value_type& get() { return m_val; }

  /// Getter (const version)
  const value_type& get() const { return m_val; }

  /// String representation for printout
  std::string toStr() const override;

  /// String representation for printout
  std::string toXMLStr( std::string_view name, std::string_view comment, int precision ) const override;

  /// TypeId of the datum
  const std::type_info& type() const override { return typeid( T ); }

  /// Create a copy of this object on the heap (generic copy)
  std::unique_ptr<BasicParam> clone() const override { return std::make_unique<Param<T>>( m_val ); }

private:
  /// Void pointer to the datum (used by BasicParam)
  void* _get_ptr() override { return &m_val; }
  /// Void pointer to the datum (used by BasicParam), const version
  const void* _get_ptr() const override { return &m_val; }
  /// Where the datum is stored
  value_type m_val;
};

#include "GaudiKernel/SerializeSTL.h"
/// String representation for printout
template <class T>
std::string Param<T>::toStr() const {
  using namespace GaudiUtils;
  std::ostringstream o;
  o << m_val;
  return o.str();
}

template <>
inline std::string Param<double>::toStr() const {
  std::ostringstream o;
  o.precision( 16 );
  o << m_val;
  return o.str();
}
namespace details {
  template <typename...>
  struct always_false : std::bool_constant<false> {};
} // namespace details

/// Default implementation. Only a limited number of types are allowed in
/// the XML representation of a ParamValidDataObject
template <class T>
std::string Param<T>::toXMLStr( std::string_view, std::string_view, int ) const {
  static_assert( ::details::always_false<T>::value, "Cannot convert parameter to XML" );
  return {};
}

// === Forward declaration of version of toXMLStr for the supported types ===
template <>
std::string Param<int>::toXMLStr( std::string_view name, std::string_view comment, int precision ) const;
template <>
std::string Param<double>::toXMLStr( std::string_view name, std::string_view comment, int precision ) const;
template <>
std::string Param<std::string>::toXMLStr( std::string_view name, std::string_view comment, int precision ) const;

template <>
std::string Param<std::vector<int>>::toXMLStr( std::string_view name, std::string_view comment, int precision ) const;
template <>
std::string Param<std::vector<double>>::toXMLStr( std::string_view name, std::string_view comment,
                                                  int precision ) const;
template <>
std::string Param<std::vector<std::string>>::toXMLStr( std::string_view name, std::string_view comment,
                                                       int precision ) const;

template <>
std::string Param<std::map<std::string, int>>::toXMLStr( std::string_view name, std::string_view comment,
                                                         int precision ) const;
template <>
std::string Param<std::map<std::string, double>>::toXMLStr( std::string_view name, std::string_view comment,
                                                            int precision ) const;
template <>
std::string Param<std::map<std::string, std::string>>::toXMLStr( std::string_view name, std::string_view comment,
                                                                 int precision ) const;

template <>
std::string Param<std::map<int, int>>::toXMLStr( std::string_view name, std::string_view comment, int precision ) const;
template <>
std::string Param<std::map<int, double>>::toXMLStr( std::string_view name, std::string_view comment,
                                                    int precision ) const;
template <>
std::string Param<std::map<int, std::string>>::toXMLStr( std::string_view name, std::string_view comment,
                                                         int precision ) const;

#endif // DETDESC_PARAM_H
