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
#ifndef EVENT_RELATEDINFOMAP_H
#define EVENT_RELATEDINFOMAP_H 1

// STL
#include <ostream>

// Gaudi
#include "GaudiKernel/VectorMap.h"

namespace LHCb {

  /** @class RelatedInfoMap Event/RelatedInfoMap.h
   *
   *  Related information map.
   *
   *  @author Anton Poluektov
   *  @date   2014-08-01
   */
  class RelatedInfoMap : public GaudiUtils::VectorMap<short, float> {

  public:
    typedef GaudiUtils::VectorMap<short, float>             MAP;
    typedef GaudiUtils::VectorMap<short, float>::_vector    VECT;
    typedef GaudiUtils::VectorMap<short, float>::value_type VAL;

  public:
    /// Has information for specified key.
    inline bool hasInfo( const short key ) const { return this->end() != this->find( key ); }

    /// Add or replace new information, associated with the key.
    inline bool addInfo( const short key, const float info ) { return this->insert( key, info ).second; }

    /** Extract the information associated with the given key.
     *  If there is no such infomration the default value will be returned.
     */
    inline double info( const short key, const float def ) const {
      const_iterator i = this->find( key );
      return ( this->end() == i ? def : i->second );
    }

    /// Erase the information associated with the given key.
    inline unsigned long eraseInfo( const short key ) { return this->erase( key ); }

  public:
    /// Overload output to ostream
    friend inline std::ostream& operator<<( std::ostream& os, const RelatedInfoMap& map ) {
      os << "[ ";
      for ( LHCb::RelatedInfoMap::const_iterator i = map.begin(); i != map.end(); ++i ) {
        os << i->first << "=" << i->second << " ";
      }
      return os << "]";
    }
  };

} // namespace LHCb
#endif // KERNEL_IRELATEDINFOTOOL_H
