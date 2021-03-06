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
#ifndef PARTICLE2LHCBIDS_H
#define PARTICLE2LHCBIDS_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/DataObject2ObjectMap.h"
#include "Kernel/LHCbID.h"

#include "Event/Particle.h"

namespace DaVinci {

  namespace Map {

    /** @class Particle2LHCbIDs Kernel/Particle2LHCbIDs.h
     *
     *
     *  @author Juan Palacios
     *  @date   2010-08-18
     */
    class Particle2LHCbIDs : public DataObject2ObjectMap<LHCb::Particle, std::vector<LHCb::LHCbID>> {

    public:
      // ======================================================================
      /// Retrieve pointer to class definition structure     (virtual)
      const CLID& clID() const override { return Particle2LHCbIDs::classID(); }

      /// Retrieve pointer to class definition structure     (static)
      static const CLID& classID();

      // ======================================================================
    };

  } // namespace Map

} // namespace DaVinci

#endif // PARTICLE2LHCBIDS_H
