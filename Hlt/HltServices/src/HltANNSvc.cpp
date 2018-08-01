#include "ANNSvc.h"
#include <string>
#include <functional>


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
    std::optional<minor_value_type> handleUndefined(const major_key_type& major, const std::string& minor) const override;
private:
    Gaudi::Property<bool> m_allowUndefined { this, "allowUndefined",true,"do we allow undefined, on-demand generated, key/value pairs?"};
};

DECLARE_COMPONENT( HltANNSvc )

std::optional<IANNSvc::minor_value_type>
HltANNSvc::handleUndefined(const major_key_type& major, const std::string& minor) const
{
   if (!m_allowUndefined)  return {};

   static const std::hash<std::string> hasher;
   debug() << "handleUndefined called for " << major << " : " << minor
           << " -->  " << hasher(minor) << endmsg;
   return minor_value_type(minor,hasher(minor));
}
