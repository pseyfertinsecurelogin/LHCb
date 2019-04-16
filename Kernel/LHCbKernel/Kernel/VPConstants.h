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
#ifndef KERNEL_VPCONSTANTS_H
#define KERNEL_VPCONSTANTS_H 1
namespace VP {

  inline constexpr unsigned int NModules          = 52;
  inline constexpr unsigned int NSensorsPerModule = 4;
  inline constexpr unsigned int NSensors          = NModules * NSensorsPerModule;
  inline constexpr unsigned int NChipsPerSensor   = 3;
  inline constexpr unsigned int NRows             = 256;
  inline constexpr unsigned int NColumns          = 256;
  inline constexpr unsigned int NSensorColumns    = NColumns * NChipsPerSensor;
  inline constexpr unsigned int NPixelsPerSensor  = NSensorColumns * NRows;

  inline constexpr double Pitch = 0.055;

} // namespace VP

#endif
