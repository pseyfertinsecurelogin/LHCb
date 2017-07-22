#include <cstdlib>
#include <memory>
#include <numeric>
#include <stdexcept>
#include "Kernel/PropertyConfig.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/IProperty.h"

namespace {
const std::string& toString( PropertyConfig::kind_t kind)  {
    static const std::string s_empty;
    static const std::string s_IAlgorithm{"IAlgorithm"};
    static const std::string s_IService{"IService"};
    static const std::string s_IAlgTool{"IAlgTool"};
    static const std::string s_IAuditor{"IAuditor"};
    switch (kind) {
        case PropertyConfig::kind_t::IAlgorithm: return s_IAlgorithm;
        case PropertyConfig::kind_t::IService: return s_IService;
        case PropertyConfig::kind_t::IAlgTool: return s_IAlgTool;
        case PropertyConfig::kind_t::IAuditor: return s_IAuditor;
        default : return s_empty; //for backwards compatibility -- for now...
    }
}
PropertyConfig::kind_t fromString(const std::string& s) {
    for (auto k : { PropertyConfig::kind_t::IAlgorithm,
                    PropertyConfig::kind_t::IService,
                    PropertyConfig::kind_t::IAlgTool } ) {
        if (s == toString(k)) return k;
    }
    return PropertyConfig::kind_t::Invalid;
}
}

const std::string& PropertyConfig::kind() const {
    return toString(m_kind);
}

PropertyConfig::Properties PropertyConfig::initProperties(const IProperty& obj) {
    const auto& items = obj.getProperties();
    Properties props; props.reserve(items.size());
    std::for_each( begin(items), end(items), [&]( const Property *i ) {
        // FIXME: check for duplicates!!!
        props.emplace_back(i->name(),i->toString());
        // verify that toString / fromString are each others inverse...
        // WARNING: this does not guarantee that we don't loose information -- toString may be lossy!!!!
        // WARNING: we remove the update handler (if any), which means that weird behaviour there (like
        //          changing the property that was just updated) will be missed.
        std::unique_ptr<Property> clone( i->clone() );
        clone->declareUpdateHandler(nullptr);
        if ( clone->fromString(props.back().second).isFailure() ) {
            std::cerr << "Property::fromString fails to parse Property::toString" << std::endl;
            std::cerr << i->type()<< " : " << i->name() << " -> " << i->toString() << std::endl;
            ::abort(); // this is REALLY bad, and we should die, die, die!!!
                       // as we won't realize this until we read back later, so we
                       // really have to make sure this is never written in the first place
        }
        if ( clone->toString() != props.back().second ) {
            std::cerr << " Property::fromString, followed by Property::toString does not give the same result:\n"
                      << "\'" <<  clone->toString() << "\' vs. \n\'" << props.back().second << "\'"
                      << std::endl;
            // this is still no guarantee everything is OK, as it checks that checks that toString is a projection operator.
            // The initial toString may be lossy, and this 'just' checks that the subsequent toString doesn't loose anything
            // more than the initial one...
            ::abort();
        }
    } );
    return props;
}

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/regex.hpp"

using boost::property_tree::ptree;
namespace {
void read_custom(std::istream& is, ptree& top) {
    bool parsing_properties = false;
    static const boost::regex propstart("^Properties: \\[$"),
                              property("^ ?'([^']+)':(.*)$"),
                              propend("^\\]$"),
                              topitem("^(Name|Type|Kind): (.*)$");
    boost::smatch what;
    std::string s;
    ptree& props = top.put_child("Properties",ptree{});
    while (std::istream::traits_type::not_eof( is.peek()) ) {
        getline(is,s);
        if (s.empty()) continue;
        if (parsing_properties) {
            if (boost::regex_match(s,what,property) ) {
                // props.put(what[1].str(),what[2].str());
                // use add instead of put as Gaudi allows multiple properties with
                // the same name ... surely an oversight...
                props.add(what[1].str(),what[2].str());
            } else if (boost::regex_match(s,what,propend) ) {
                parsing_properties = false;
            } else {
                std::cerr << "parsing error while looking for properties!!! : [" << s << "]" << std::endl;
            }
        } else {
            if (boost::regex_match(s,what,topitem) ) {
                top.put(what[1].str(),what[2].str());
            } else if (boost::regex_match(s,what,propstart) ) {
                assert(props.empty());
                parsing_properties = true;
            } else {
                std::cerr << "parsing error!!! : [" << s << "]" << std::endl;
            }
        }
    }
}
}

std::istream& PropertyConfig::read(std::istream& is) {
    m_digest = digest_type::createInvalid(); // reset our state;
    ptree top;
    switch (is.peek()) {
        case '{' : read_json(is,top);   break;
        case '<' : read_xml(is,top);    break;
        default  : read_custom(is,top); break;
    }
    if (top.empty()) {
        // flag as invalid 'forever'
        m_properties.clear();
        m_name.clear();
        m_kind = kind_t::Invalid;
        m_type.clear();
    } else {
        const ptree& props = top.get_child("Properties");
        m_properties.clear(); m_properties.reserve( props.size() );
        std::transform( begin(props), end(props),
                        std::back_inserter(m_properties),
                        []( const ptree::value_type& i )
                        { return std::make_pair( i.first, i.second.data() ); } );
        m_type = top.get<std::string>("Type");
        m_name = top.get<std::string>("Name");
        m_kind = fromString(top.get<std::string>("Kind"));
        m_digest = getDigest();
    }
    return is;
}

std::string PropertyConfig::str() const {
    // this is the 'original' canonical representation that MUST be used to compute the hash....
    // (unless we 'convert' & 're-index' the already written data to any new format)
    // as a result, do NOT change the result of this function!!!
    std::string out;
    out.reserve(std::accumulate( begin(properties()), end(properties()),
                                 name().size()+kind().size()+type().size()+37,
                                 [](int s, const Prop& i) {
                        return s+5+i.first.size()+i.second.size();
    } ) ) ;
    out +=  "Name: "; out += name(); out += '\n';
    out +=  "Kind: "; out += kind(); out += '\n';
    out +=  "Type: "; out += type(); out += '\n';
    out +=  "Properties: [\n";
    std::for_each( begin(properties()), end(properties()), [&out]( const Prop& i ) {
        out +=  " '"; out+= i.first ; out += "':";
        // remove all newlines... as we use newline as a record seperator (and yes, this
        // should have been escaped instead... didn't think of that when I originally wrote
        // it...)
        std::copy_if( begin(i.second), end(i.second),
                      std::back_inserter(out),
                      [](const char x) { return x!='\n'; } );
        out += '\n';
    } );
    out += "]\n" ;
    return out;
}

std::ostream& PropertyConfig::print(std::ostream& os) const {
    // note: advantage of json (or xml): in case of hash collision, can add an optional extra field...
    // but that only works if the json representation is used to compute the digest -- which isn't the
    // case!!! (not impossible, but requires a very carefull, non-trivial re-persisting of the already
    // persistent config data...)
    ptree top;
    top.put("Name",name());
    top.put("Kind",kind());
    top.put("Type",type());
    std::transform( begin(properties()), end(properties()),
                    std::back_inserter(top.put_child("Properties",ptree{})),
                    []( const Prop& i ) { return std::make_pair(i.first,ptree{i.second}); } );
    write_json(os,top,false);
    return os;
}
