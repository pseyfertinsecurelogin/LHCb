#ifndef ASSOCIATORS_LOCATION_H
#define ASSOCIATORS_LOCATION_H

#include <string>

namespace Links {
inline std::string location(const std::string& location)
{
   return "Link/" + (location.compare(0, 7, "/Event/") == 0 ? location.substr(7) : location);
}
}

#endif
