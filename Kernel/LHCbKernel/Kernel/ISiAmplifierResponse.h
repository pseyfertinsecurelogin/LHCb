/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _ISiAMPLIFIERRESPONSE_H
#define _ISiAMPLIFIERRESPONSE_H

#include "GaudiKernel/IAlgTool.h"
#include <string>


namespace SiAmpliferResponseType{

  static const std::string& signal = "signal";
  static const std::string& capCoupling = "capCoupling";
}

/** @class ISiAmplifierResponse ISiAmplifierResponse.h Kernel/ISiAmplifierResponse.h
 *
 *  Interface Class for estimating response of amplifier in Si Channel
 *
 *  @author M.Needham
 *  @date   6/11/2005
 */
struct ISiAmplifierResponse : extend_interfaces<IAlgTool> {

  DeclareInterfaceID(ISiAmplifierResponse, 1 , 0);

  struct Info final {
    double capacitance;
    unsigned int vfs;
    std::string type;
  };


  /** calculate Beetle response
  * @param  time time in ns
  * @return response
  */
  virtual double response(const double time) const=0;

  /** The response is only valid for a certain capacitance, Vfs, etc.
  * This method allows you to find out when the curve is valid
  *
  * @return validity info
  */
  virtual ISiAmplifierResponse::Info validity() const = 0;

  /** calculate the remainder ie signal left after 25 ns
  * @param time time of sampling relative to peak [default is zero ]
  * @return response
  */
  virtual double remainder(double time = 0) const = 0;

  /** calculate the rise time [10 - 90 %]
  * @return ristime
  */
  virtual double risetime() const = 0;

};

#endif // _ISiAMPLIFIERRESPONSE_H
