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
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "GaudiKernel/VectorMap.h"
#include "GaudiKernel/StringKey.h"

//declaration of the parser
namespace Gaudi { namespace Parsers {
StatusCode parse(GaudiUtils::VectorMap<Gaudi::StringKey, int>& result, const std::string& input) ;
StatusCode parse(std::map<Gaudi::StringKey, int>& result, const std::string& input) ;
  }
}

#include "ANNSvc.h"
#include <vector>
#include <map>
#include <utility>
#include "GaudiKernel/Property.h"


using std::map;
using std::vector;
using std::string;

namespace ANNSvcUtilities {

  template <typename KEY, typename VALUE>
  class bimap_t {
  public:
    typedef KEY                                  key_type;
    typedef VALUE                                value_type;
    typedef std::pair<KEY,VALUE>                 mapped_type;

    typedef GaudiUtils::VectorMap<KEY,VALUE>     k2v_type;
    typedef GaudiUtils::VectorMap<VALUE,KEY>     v2k_type;
    typedef SimpleProperty<std::map<KEY,VALUE> > property_type;
    typedef std::optional<IANNSvc::minor_value_type>  result_type;

    bimap_t(const ANNSvc* parent);

    property_type& property() { return m_property; }

    result_type value(const key_type& key) const {
      auto i = m_map.find(key);
      return i == m_map.end() ? result_type()
                              : result_type(*i) ;
    }

    result_type   key(const value_type& value) const {
      auto i = m_invmap.find(value);
      return i == m_invmap.end() ? result_type{}
                                 : result_type{std::pair{i->second,i->first}} ;
    }

    const k2v_type& mapping() const { return m_map; }

    void insert(const mapped_type& value);

  private:
    bimap_t(const bimap_t& rhs);
    void updateHandler(Property& prop);

    property_type   m_property;
    k2v_type        m_map;
    v2k_type        m_invmap;
    const ANNSvc*   m_parent;
  };


  template<typename KEY, typename VALUE>
  bimap_t<KEY,VALUE>::bimap_t(const ANNSvc* parent) : m_parent(parent) {
    m_property.declareUpdateHandler( &bimap_t<KEY,VALUE>::updateHandler, this);
  }

  template <typename KEY, typename VALUE>
  bimap_t<KEY,VALUE>::bimap_t(const bimap_t& rhs)
    : m_map(       rhs.m_map      )
    , m_invmap(    rhs.m_invmap   )
    , m_property(  rhs.m_property )
    , m_parent(    rhs.m_parent   )
  {
    m_property.declareUpdateHandler(&bimap_t<KEY,VALUE>::updateHandler, this);
  }

  template <typename KEY, typename VALUE>
  void
  bimap_t<KEY,VALUE>::insert(const mapped_type& value) {
    // verify each entry to be unique
    if (m_map.find( value.first ) != m_map.end() ||
        m_invmap.find(value.second) != m_invmap.end() ) {
      if (m_parent) {
        m_parent->error() << " Entry \"" << value.first
                          << "\" -> \""<< value.second
                          <<"\" clashes with existing entry" << endmsg;
      }
      throw GaudiException( "non-unique entry in ANNSvc instance","",StatusCode::FAILURE);
    }
    m_map.   insert( value.first,  value.second );
    m_invmap.insert( value.second, value.first  );
  }

  template <typename KEY, typename VALUE>
  void
  bimap_t<KEY,VALUE>::updateHandler(Property& /*prop*/) {
    // grab the one we made ourselves and preserve them unless overruled???
    m_map.   clear();
    m_invmap.clear();
    for ( const auto& i : m_property.value() ) insert(i);
  }

}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ANNSvc::ANNSvc( const string& name, ISvcLocator* pSvcLocator,
                const vector<IANNSvc::major_key_type>& majors )
  : base_class( name , pSvcLocator )
{
  for (const auto& i  : majors ) {
    auto r = m_maps.insert(i, new bimap_type(this));
    declareProperty(r.first->first,
                    const_cast<maps_type::mapped_type&>(r.first->second)->property());
  }
}

//=============================================================================
// finalize
//=============================================================================
StatusCode
ANNSvc::finalize() {
  for( auto i : m_maps ) {
    const auto& prop = i.second->property().value();
    for (const auto& j : i.second->mapping() ) {
      if ( prop.find(j.first) != prop.end() ) continue;
      info() << " autogenerated: " << i.first << " / " << j.first << " : " << j.second << endmsg;
    }
  }
  return StatusCode::SUCCESS;
}


//=============================================================================
// IANNSvc interface implementation
//=============================================================================
bool
ANNSvc::hasMajor(const IANNSvc::major_key_type& major) const {
  return m_maps.find(major)!=m_maps.end();
}

std::optional<IANNSvc::minor_value_type>
ANNSvc::handleUndefined(const IANNSvc::major_key_type&, int ) const {
  return {};
}

std::optional<IANNSvc::minor_value_type>
ANNSvc::handleUndefined(const IANNSvc::major_key_type&, const std::string& ) const {
  return {};
}

std::optional<IANNSvc::minor_value_type>
ANNSvc::value(const IANNSvc::major_key_type& major, const string& minor) const {
  auto i = m_maps.find(major);
  if (i==m_maps.end()) return {};
  auto x = i->second->value(minor);
  if (!x) {
    x = handleUndefined(major,minor);
    if (x) i->second->insert(*x);
  }
  return x;
}

std::optional<IANNSvc::minor_value_type>
ANNSvc::value(const IANNSvc::major_key_type& major, int minor) const {
  auto i = m_maps.find(major);
  if (i==m_maps.end()) return {};
  auto x = i->second->key(minor);
  if (!x) {
    x = handleUndefined(major,minor);
    if (x) i->second->insert(*x);
  }
  return x;
}

std::vector<IANNSvc::minor_key_type>
ANNSvc::keys(const IANNSvc::major_key_type& major) const {
  auto i = items(major);
  std::vector<IANNSvc::minor_key_type>  k; k.reserve(i.size());
  std::transform(i.begin(), i.end(),
                 std::back_inserter(k),
                 [](const minor_value_type& i) { return i.first; } );
  return k;
}

std::vector<IANNSvc::minor_value_type>
ANNSvc::items(const IANNSvc::major_key_type& major) const {
  std::vector<IANNSvc::minor_value_type>  r;
  auto i = m_maps.find(major);
  if (i!=m_maps.end())  {
    r.reserve( i->second->mapping().size() );
    r.insert( r.end(), i->second->mapping().begin(),
              i->second->mapping().end() );
  }
  return r;
}

// new functionality requested in task #48818
GaudiUtils::VectorMap< IANNSvc::minor_value_type::first_type, IANNSvc::minor_value_type::second_type >
ANNSvc::item_map(const IANNSvc::major_key_type& major) const
{
  //create the map to return
  GaudiUtils::VectorMap< IANNSvc::minor_value_type::first_type, IANNSvc::minor_value_type::second_type > r;
  auto i = m_maps.find(major);
  if (i!=m_maps.end())
  {
    r.reserve( i->second->mapping().size() );
    r.merge(i->second->mapping());

  }
  return r;
}

std::vector<IANNSvc::major_key_type>
ANNSvc::majors() const {
  std::vector< major_key_type > r; r.reserve(m_maps.size());
  std::transform( m_maps.begin(), m_maps.end(),
                  std::back_inserter(r),
                  [](maps_type::const_reference i) { return i.first; } );
  return r;
}
// ============================================================================
// BEGIN new parser:
// ============================================================================
// STD && STL:
// ============================================================================
#include <map>
#include <string>
// ============================================================================
// Gaudi
// ============================================================================
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ParsersFactory.h"
// ============================================================================
namespace Gaudi {
namespace Parsers {
StatusCode parse(GaudiUtils::VectorMap<Gaudi::StringKey, int>& result, const std::string& input)
{
    return Gaudi::Parsers::parse_(result, input);
}
StatusCode parse(std::map<Gaudi::StringKey, int>& result, const std::string& input)
{
    return Gaudi::Parsers::parse_(result, input);
}
// ============================================================================
// END of parser
// ============================================================================

} }
