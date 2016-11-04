// $Id: Particle2UnsignedInts.h,v 1.1 2010/08/19 15:09:43 jpalac Exp $
#ifndef PARTICLE2UNSIGNEDINTS_H
#define PARTICLE2UNSIGNEDINTS_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/DataObject2ObjectMap.h"
#include "Event/Particle.h"

namespace DaVinci{

  namespace Map {


    /** @class Particle2UnsignedInts Kernel/Particle2UnsignedInts.h
     *
     *
     *  @author Juan Palacios
     *  @date   2010-09-27
     */
    class Particle2UnsignedInts : public DataObject2ObjectMap< LHCb::Particle,
                                                               std::vector<unsigned int> >
    {

    public:

      // ======================================================================
      /// Retrieve pointer to class definition structure     (virtual)
      const CLID& clID() const override
      {
        return Particle2UnsignedInts::classID() ;
      }

      /// Retrieve pointer to class definition structure     (static)
      static const CLID& classID();

      // ======================================================================

    };

  } // namespace Map

} // namespace DaVinci


#endif // PARTICLE2UNSIGNEDINTS_H
