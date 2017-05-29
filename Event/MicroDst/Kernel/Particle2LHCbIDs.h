// $Id: Particle2LHCbIDs.h,v 1.1 2010/08/19 15:09:43 jpalac Exp $
#ifndef PARTICLE2LHCBIDS_H
#define PARTICLE2LHCBIDS_H 1

#include <vector>

#include "Event/Particle.h"
#include "GaudiKernel/ClassID.h"
// Include files
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/DataObject2ObjectMap.h"
#include "Kernel/LHCbID.h"

namespace LHCb {
class LHCbID;
class Particle;
}  // namespace LHCb

namespace DaVinci{

  namespace Map {


    /** @class Particle2LHCbIDs Kernel/Particle2LHCbIDs.h
     *
     *
     *  @author Juan Palacios
     *  @date   2010-08-18
     */
    class Particle2LHCbIDs : public DataObject2ObjectMap< LHCb::Particle,
                                                          std::vector<LHCb::LHCbID> >
    {

    public:

      // ======================================================================
      /// Retrieve pointer to class definition structure     (virtual)
      const CLID& clID() const override
      {
        return Particle2LHCbIDs::classID() ;
      }

      /// Retrieve pointer to class definition structure     (static)
      static const CLID& classID();

      // ======================================================================

    };

  } // namespace Map

} // namespace DaVinci


#endif // PARTICLE2LHCBIDS_H
