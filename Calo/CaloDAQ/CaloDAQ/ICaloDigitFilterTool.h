#ifndef ICALODIGITFILTERTOOL_H 
#define ICALODIGITFILTERTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "Event/CaloDigit.h"
#include "GaudiKernel/IAlgTool.h"


/** @class ICaloDigitFilterTool ICaloDigitFilterTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-13
 */
struct ICaloDigitFilterTool : extend_interfaces<IAlgTool>
{

  // Return the interface ID
  DeclareInterfaceID( ICaloDigitFilterTool, 3, 0 );

  virtual int getScale()=0;
  virtual bool cleanDigits(const std::string& det, bool substr=true, bool mask = true,bool spd=false)=0;
  virtual int method(const std::string& det)=0;
  virtual double offset(LHCb::CaloCellID id,bool spd=false)=0;    
  virtual double offsetRMS(LHCb::CaloCellID id,bool spd=false)=0;    

};

#endif // ICALODIGITFILTERTOOL_H
