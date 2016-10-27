#ifndef _DEUTDETECTOR_H_
#define _DEUTDETECTOR_H_

#include "STDet/DeSTDetector.h"
#include "Kernel/STChannelID.h"

class DeSTSector;

/** @class DeUTDetector DeUTDetector.h STDet/DeUTDetector.h
 *
 *  UT Detector Element class
 *
 *  All UT elements are modification from TT element classes
 *  that were originally written by Matt Needham)
 *
 *  @author Jianchun Wang, Matt Needham
 *  @date   2012-07-10
 *
 */

static const CLID CLID_DeUTDetector = 9301;

class DeUTDetector : public DeSTDetector  {

public:

  /** Constructor */
  DeUTDetector ( const std::string& name = "" ) ;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID(){return CLID_DeUTDetector;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /**
  *  short cut to pick up the wafer corresponding to x,y,z
  * @param  aPoint point in global frame
  * @return sector
  */
  DeSTSector* findSector(const Gaudi::XYZPoint& aPoint) const override;

 /**
  *  find top level DeSTBaseElement by nickname
  * @param nickname
  * @return base element
  */
  DeSTBaseElement* findTopLevelElement(const std::string& nickname) const override;

private:

  /** make flat list of lowest descendents  and also layers */
  void flatten();

};

#endif // _DEUTDETECTOR_H
