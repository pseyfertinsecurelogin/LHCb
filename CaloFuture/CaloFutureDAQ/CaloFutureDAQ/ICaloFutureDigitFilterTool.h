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
#ifndef ICALOFUTUREDIGITFILTERTOOL_H 
#define ICALOFUTUREDIGITFILTERTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "Event/CaloDigit.h"
#include "GaudiKernel/IAlgTool.h"


/** @class ICaloFutureDigitFilterTool ICaloFutureDigitFilterTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-13
 */
struct ICaloFutureDigitFilterTool : extend_interfaces<IAlgTool>
{

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureDigitFilterTool, 3, 0 );

  virtual int getScale()=0;
  virtual bool cleanDigits(const std::string& det, bool substr=true, bool mask = true,bool spd=false)=0;
  virtual int method(const std::string& det)=0;
  virtual double offset(LHCb::CaloCellID id,bool spd=false)=0;    
  virtual double offsetRMS(LHCb::CaloCellID id,bool spd=false)=0;    

};

#endif // ICALOFUTUREDIGITFILTERTOOL_H
