#include <boost/functional/hash/hash.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional.hpp>
#include <functional>
#include <string>

#include "ANNSvc.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "Gaudi/PluginService.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/StringKey.h"
#include "Kernel/IANNSvc.h"

class ISvcLocator;


class HltANNSvc : public ANNSvc {
public:
    HltANNSvc( const std::string& name, ISvcLocator *svcLocator )
    :ANNSvc(name, svcLocator, {{"Hlt1SelectionID"},
                               {"Hlt2SelectionID"},
                               {"InfoID"},
                               {"RoutingBits"},
                               {"RelInfoLocations"},
                               {"PackedObjectLocations"}})
    {
    }
    using ANNSvc::handleUndefined;
    boost::optional<minor_value_type> handleUndefined(const major_key_type& major, const std::string& minor) const override;
private:
    Gaudi::Property<bool> m_allowUndefined { this, "allowUndefined",true,"do we allow undefined, on-demand generated, key/value pairs?"};
};

DECLARE_COMPONENT( HltANNSvc )

boost::optional<IANNSvc::minor_value_type>
HltANNSvc::handleUndefined(const major_key_type& major, const std::string& minor) const
{
   if (!m_allowUndefined)  return boost::none;

   static boost::hash<std::string> hasher;
   debug() << "handleUndefined called for " << major << " : " << minor
           << " -->  " << hasher(minor) << endmsg;
   return minor_value_type(minor,hasher(minor));
}
