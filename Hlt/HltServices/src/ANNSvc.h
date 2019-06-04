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
#ifndef ANNSVC_H
#define ANNSVC_H 1

// Include files
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/StringKey.h"
#include "Kernel/IANNSvc.h"
#include "boost/container/flat_map.hpp"
#include <string>

class ANNSvc;

// declaration of the parser
namespace Gaudi {
  namespace Parsers {
    StatusCode parse( std::map<Gaudi::StringKey, int>& result, const std::string& input );
  } // namespace Parsers
} // namespace Gaudi

namespace details::ANNSvc {

  template <typename KEY, typename VALUE>
  class bimap_t {
  public:
    typedef KEY                   key_type;
    typedef VALUE                 value_type;
    typedef std::pair<KEY, VALUE> mapped_type;

    typedef GaudiUtils::VectorMap<KEY, VALUE>        k2v_type;
    typedef GaudiUtils::VectorMap<VALUE, KEY>        v2k_type;
    typedef Gaudi::Property<std::map<KEY, VALUE>>    property_type;
    typedef std::optional<IANNSvc::minor_value_type> result_type;

    bimap_t( const ::ANNSvc* parent );

    property_type& property() { return m_property; }

    result_type value( const key_type& key ) const {
      auto i = m_map.find( key );
      return i == m_map.end() ? result_type() : result_type( *i );
    }

    result_type key( const value_type& value ) const {
      auto i = m_invmap.find( value );
      return i == m_invmap.end() ? result_type{} : result_type{std::pair{i->second, i->first}};
    }

    const k2v_type& mapping() const { return m_map; }

    void insert( const mapped_type& value );

  private:
    bimap_t( const bimap_t& rhs );
    void updateHandler( Property& prop );

    property_type   m_property;
    k2v_type        m_map;
    v2k_type        m_invmap;
    const ::ANNSvc* m_parent;
  };
} // namespace details::ANNSvc
/** @class ANNSvc ANNSvc.h
 *
 *  functionality:
 *     maps between 'Assigned Names and Numbers'
 *         and also 'Assigned Numbers and Names'
 *
 *  @author Gerhard Raven
 *  @date   2008-01-02
 */

class ANNSvc : public extends<Service, IANNSvc> {
public:
  ANNSvc( const std::string& name, ISvcLocator* pSvcLocator, const std::vector<major_key_type>& majors );

  StatusCode finalize() override;

  std::optional<minor_value_type> value( const major_key_type& major, const std::string& minor ) const override;
  std::optional<minor_value_type> value( const major_key_type& major, int minor ) const override;

  bool hasMajor( const major_key_type& major ) const override;

  std::vector<minor_key_type>   keys( const major_key_type& major ) const override;
  std::vector<minor_value_type> items( const major_key_type& major ) const override;
  GaudiUtils::VectorMap<minor_value_type::first_type, minor_value_type::second_type>
                              item_map( const major_key_type& major ) const override;
  std::vector<major_key_type> majors() const override;

protected:
  // Call handleUndefined if an unknown key/value is requested...
  //  default implementation just returns an unitialized std::optional
  virtual std::optional<minor_value_type> handleUndefined( const major_key_type& major, int minor ) const;
  virtual std::optional<minor_value_type> handleUndefined( const major_key_type& major,
                                                           const std::string&    minor ) const;

private:
  typedef details::ANNSvc::bimap_t<minor_key_type, minor_mapped_type>             bimap_type;
  typedef boost::container::flat_map<major_key_type, std::unique_ptr<bimap_type>> maps_type;
  maps_type                                                                       m_maps;
  friend class details::ANNSvc::bimap_t<minor_key_type, minor_mapped_type>;
};

#endif // ANNSVC_H 1
