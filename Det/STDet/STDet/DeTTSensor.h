#ifndef _DeTTSensor_H_
#define _DeTTSensor_H_

#include <string>

#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/STChannelID.h"
#include "STDet/DeSTSensor.h"
#include "STDet/DeTTSector.h"
#include "STDet/STDetTraits.h"

/** @class DeTTSensor DeTTSensor.h "STDet/DeTTSensor.h"
 *
 *  Class representing a TT Sensor
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 *   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *
 */

class DeSTSector;

static const CLID CLID_DeTTSensor = 9130;

class DeTTSensor : public DeSTSensor  {

public:

  /** parent type */
  typedef STDetTraits<DeTTSensor>::parent parent_type;

  /** Constructor */
  DeTTSensor ( const std::string& name = "" ) ;

  /**
  * Retrieves reference to class identifier
  * @return the class identifier for this class
  */
  static const CLID& classID(){return CLID_DeTTSensor;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

private:

  parent_type* m_parent;

};


#endif // _DeTTSensor_H








