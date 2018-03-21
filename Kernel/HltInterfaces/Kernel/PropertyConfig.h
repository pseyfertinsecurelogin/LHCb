#ifndef PROPERTYCONFIG_H
#define PROPERTYCONFIG_H 1
#include <string>
#include <tuple> // for std::tie
#include <vector>
#include <iostream>
#include <numeric>
#include <string_view>
#include "boost/operators.hpp"
#include "GaudiKernel/System.h"
#include "GaudiKernel/INamedInterface.h"
#include "GaudiKernel/IProperty.h"
#include "LHCbMath/MD5.h"

namespace {
    struct modify_fn {
      template <typename Properties>
      Properties operator()(Properties props, std::string_view key, std::string_view value) const {
          auto i = std::find_if( begin(props), end(props),
                                 [key](const auto& p) { return p.first == key; } );
          if (i==end(props)) {
              throw std::invalid_argument( "PropertyConfig: trying to update unknown property '"
                                           +std::string{key}+"'");
          }
          i->second = value;
          return props;
      }
      template <typename Properties>
      Properties operator()(Properties props, std::string_view keyAndValue) const {
          auto c = keyAndValue.find(':');
          if (c == std::string_view::npos )
              throw std::invalid_argument( "PropertyConfig: keyAndValue must contain ':'");
          return (*this)(std::move(props),keyAndValue.substr(0,c),keyAndValue.substr(c+1));
      }
      template <typename Properties>
      Properties operator()(Properties props, const std::pair<std::string,std::string>& p) const {
          return (*this)(std::move(props),p.first,p.second);
      }
      template <typename Properties, typename Iter,
                // require Iter to be an iterator:
                typename = typename std::iterator_traits<Iter>::iterator_category>
      Properties operator()(Properties props, Iter first, Iter last) const {
          for(;first!=last;++first) props = (*this)(std::move(props),*first);
          return props;
      }
    };

    constexpr modify_fn modify{};
}

class PropertyConfig final : public boost::equality_comparable<PropertyConfig> {
public:
    typedef Gaudi::Math::MD5 digest_type;
    typedef std::pair<std::string,std::string> Prop;
    typedef std::vector<Prop> Properties;
    enum class kind_t { Invalid = -1, IAlgorithm, IService, IAlgTool, IAuditor };

    PropertyConfig() = default;

    PropertyConfig(std::string name, const IProperty& obj, kind_t kind)
      : m_properties{ initProperties(obj) }
      , m_type(System::typeinfoName(typeid(obj)))
      , m_name(std::move(name))
      , m_kind(kind)
    {}


    PropertyConfig(std::string name, std::string type, kind_t kind, Properties props)
      : m_properties(std::move(props))
      , m_type(std::move(type))
      , m_name(std::move(name))
      , m_kind(kind)
    { }

    PropertyConfig(const PropertyConfig& orig, Properties properties)
      : m_properties( std::move(properties) )
      , m_type( orig.m_type )
      , m_name( orig.m_name )
      , m_kind( orig.m_kind )
    { }

    friend bool operator==(const PropertyConfig& lhs, const PropertyConfig& rhs) {
      return std::tie( lhs.m_type, lhs.m_name, lhs.m_kind, lhs.m_properties ) ==
             std::tie( rhs.m_type, rhs.m_name, rhs.m_kind, rhs.m_properties );
    }

    const std::string& name() const    { return m_name;}
    std::string  fullyQualifiedName() const    { return type() + "/" + name();}
    const std::string& type() const    { return m_type;}
    const std::string& kind() const;
    const Properties& properties() const { return m_properties;}

    template <typename... Args>
    PropertyConfig copyAndModify(Args&&... args) const
    { return { *this, modify(properties(),std::forward<Args>(args)...)} ;}

    std::ostream& print(std::ostream& os) const;
    std::istream& read(std::istream& is);

    digest_type digest() const { return m_digest; }

private:

    friend class ConfigArchiveAccessSvc; // provide access to 'str' to allow backwards compatible writes...
    std::string str() const;

    Properties   m_properties;
    std::string  m_type, m_name;
    kind_t       m_kind = kind_t::Invalid;
    // digest must be last datamember, so it is initialized after everything else!
    digest_type  m_digest = { (m_type.empty()||m_name.empty()||m_kind == kind_t::Invalid)
                                ? digest_type::createInvalid()
                                : digest_type::compute(str()) };

    static Properties initProperties(const IProperty&);
};

inline std::ostream& operator<<(std::ostream& os, const PropertyConfig& x) { return x.print(os);}
inline std::istream& operator>>(std::istream& is, PropertyConfig& x) { return x.read(is); }
#endif
