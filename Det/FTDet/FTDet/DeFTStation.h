// $Id: $
#ifndef DEFTSTATION_H
#define DEFTSTATION_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"

/** @class DeFTStation DeFTStation.h "FTDet/DeFTStation.h"
 *
 *  This is the detector element class of the Fibre Tracker (FT) station.
 *  Currently it has no real function, but it has been created to maintain
 *  1-to-1 correspondence between the FT xml DDDB and the det. element
 *  classes in the Det/FTDet package.
 *
 *  @author Plamen Hopchev
 *  @date   2012-04-25
 */

static const CLID CLID_DeFTStation = 8602;

class DeFTStation : public DetectorElement {

public: 

  /// Standard constructor
  DeFTStation( std::string name = "" );
  
  /** Initialization method 
   *  @return Status of initialization
   */ 
  virtual StatusCode initialize();

  /** Retrieves reference to class identifier
   *  @return The class identifier for this class
   */
  const CLID& clID() const;

  /** Another reference to class identifier
   *  @return The class identifier for this class
   */
  static const CLID& classID() { return CLID_DeFTStation; }

};
#endif // DEFTSTATION_H
