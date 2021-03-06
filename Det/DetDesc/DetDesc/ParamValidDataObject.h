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
#ifndef DETDESC_PARAMVALIDDATAOBJECT_H
#define DETDESC_PARAMVALIDDATAOBJECT_H 1

// Base classes
#include "DetDesc/ValidDataObject.h"

#include "DetDesc/ParamException.h"
#include "DetDesc/ParamList.h"

#include <any>

/** @class ParamValidDataObject ParamValidDataObject.h DetDesc/ParamValidDataObject.h
 *
 *  A DataObject implementing the IParamSetEditor and IValidity
 *  interfaces. This is the basic type of condition data and detector
 *  elements in the transient data store
 *
 *  @author Sebastien Ponce
 *  @author Andrea Valassi
 *  @author Marco Clemencic
 */
class ParamValidDataObject : public ValidDataObject {

public:
  /// Default constructor
  ParamValidDataObject() = default;

  /// Copy constructor
  ParamValidDataObject( const ParamValidDataObject& obj );

public:
  /// Reset to the initial state.
  void reset();

public:
  // Re-implemented from ValidDataObject
  using ValidDataObject::update; // due to partial override
  /// Do the deep copy
  void update( ValidDataObject& obj ) override;

  /// Fill the output stream (ASCII)
  std::ostream& fillStream( std::ostream& s ) const override;

public:
  /// Check if the parameter with the given name exists.
  bool exists( const std::string& name ) const;

  /// TypeId of the parameter.
  virtual const std::type_info& type( const std::string& name ) const;

  /// Check the type of a parameter (commodity function).
  template <class T>
  bool is( const std::string& name ) const {
    return type( name ) == typeid( T );
  }

  /// Check if the parameter is a vector.
  bool isVector( const std::string& name ) const;

  /// Get the comment of a parameter.
  virtual std::string comment( const std::string& name ) const;

  /// Set the comment of a parameter.
  virtual void setComment( const std::string& name, std::string_view comm );

  /// Give a read-only accessor to a parameter.
  template <class T>
  const T& param( const std::string& name ) const {
    auto i = m_paramList.find( name );
    if ( !i ) throw ParamException( name );
    try {
      return i->get<T>();
    } catch ( const std::bad_cast& ) { throw ParamException( name, typeid( T ), i->type() ); }
  }
  template <typename... T, typename... Names>
  std::tuple<T const&...> params( Names&&... names ) const {
    static_assert( sizeof...( T ) == sizeof...( Names ) );
    return {param<T>( std::forward<Names>( names ) )...};
  }

  /// Give a read/write accessor to a parameter.
  template <class T>
  T& param( const std::string& name ) {
    auto i = m_paramList.find( name );
    if ( !i ) throw ParamException( name );
    try {
      return i->get<T>();
    } catch ( const std::bad_cast& ) { throw ParamException( name, typeid( T ), i->type() ); }
  }

  /// Give a read-only accessor to a parameter vector.
  template <class T>
  const std::vector<T>& paramVect( const std::string& name ) const {
    return param<std::vector<T>>( name );
  }

  /// Give a read/write accessor to a parameter vector.
  template <class T>
  std::vector<T>& paramVect( const std::string& name ) {
    return param<std::vector<T>>( name );
  }

  /// Get the value of a parameter, as a string.
  virtual const std::string& paramAsString( const std::string& name ) const;

  /// Get the value of a parameter, as a string (non const version).
  virtual std::string& paramAsString( const std::string& name );

  /// Get the value of a parameter, as an int.
  virtual int paramAsInt( const std::string& name ) const;

  /// Get the value of a parameter, as a double.
  virtual double paramAsDouble( const std::string& name ) const;

  /// Get the value of a parameter, as a string.
  virtual const std::vector<std::string>& paramAsStringVect( const std::string& name ) const;

  /// Get the value of a parameter, as a string.
  virtual std::vector<std::string>& paramAsStringVect( const std::string& name );

  /// Get the value of a parameter, as an int.
  virtual const std::vector<int>& paramAsIntVect( const std::string& name ) const;

  /// Get the value of a parameter, as an int.
  virtual std::vector<int>& paramAsIntVect( const std::string& name );

  /// Get the value of a parameter, as a double.
  virtual const std::vector<double>& paramAsDoubleVect( const std::string& name ) const;

  /// Get the value of a parameter, as a double.
  virtual std::vector<double>& paramAsDoubleVect( const std::string& name );

  /// Returns list of existing parameter vectors as a vector of their names
  virtual std::vector<std::string> paramNames() const;

  /// Print the parameters on a string.
  virtual std::string printParams() const;

  /// Convert a parameter to a string (for xml representation).
  virtual std::string paramToString( const std::string& name, int precision ) const;

public:
  template <class T>
  void addParam( const std::string& name, T value, std::string comment = {} ) {
    m_paramList.add( name, std::move( value ) );
    if ( !comment.empty() ) { m_comments[name] = std::move( comment ); }
  }

  void addBasicParam( const std::string& name, const BasicParam& p, std::string comment = {} ) {
    m_paramList.addBasicParam( name, p );
    if ( !comment.empty() ) { m_comments[name] = std::move( comment ); }
  }

private:
  ParamList                          m_paramList;
  std::map<std::string, std::string> m_comments;

public:
  std::any payload;
};

#endif // DETDESC_PARAMVALIDDATAOBJECT_H
