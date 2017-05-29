#ifndef MCEVENT_MCTRACKGEOMCRITERIA_H
#define MCEVENT_MCTRACKGEOMCRITERIA_H 1

// boost
#include <boost/function.hpp>
#include <boost/function/function_template.hpp>
#include <map>
// stl
#include <string>
#include <utility>
#include <vector>

// Event
#include "Event/MCTrackInfo.h"

class MCTrackInfo;

namespace LHCb
{
  class MCParticle;
}

namespace LHCb
{
  namespace MC
  {

    class MCTrackGeomCriteria
    {

    public:

      typedef boost::function<bool(MCTrackInfo&,const LHCb::MCParticle*)> Criteria;

      /** Constructor from vector of Criteria */
      MCTrackGeomCriteria( const std::vector<Criteria>& criteria );

      /** Constructor from vector of strings */
      MCTrackGeomCriteria( const std::vector<std::string>& criteria );

      /** destructor */
      ~MCTrackGeomCriteria() = default;

      /** fufulls Criteria or not */
      bool accepted(MCTrackInfo& info, const LHCb::MCParticle* particle) const;

      class FunctionMap
      {

      public:

        /// map to enum
        Criteria toType(const std::string& aName) const;

        /// destructer
        ~FunctionMap() = default;

        friend class MCTrackGeomCriteria;

      private:

        /// constructer
        FunctionMap();
        FunctionMap(const FunctionMap& rhs)
        {
          m_mapping = rhs.m_mapping;
        }

        typedef std::map<std::string, const Criteria> funMap;
        mutable funMap m_mapping;
      };

      FunctionMap& theMap()
      {
        static FunctionMap sMap;
        return sMap;
      }

    private:

      std::vector<Criteria> m_criteria;

    };
  }
}

inline LHCb::MC::MCTrackGeomCriteria::
MCTrackGeomCriteria(const std::vector<MCTrackGeomCriteria::Criteria>& criteria):
  m_criteria(criteria) { }

inline LHCb::MC::MCTrackGeomCriteria::Criteria
LHCb::MC::MCTrackGeomCriteria::FunctionMap::toType(const std::string& aName) const
{
  //  map to enum
  const auto iter = m_mapping.find(aName);
  return ( iter != m_mapping.end() ? iter->second : Criteria() );
}

#endif
