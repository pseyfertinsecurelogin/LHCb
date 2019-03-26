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
//====================================================================
#ifndef MDF_ONLINERUNINFO_H
#define MDF_ONLINERUNINFO_H

#ifdef _WIN32
#  pragma pack( push, onlineruninfo_aligment, 1 )
#  define ONLINERUNINFO_ALIGN( x ) x
#else
#  define ONLINERUNINFO_ALIGN( x ) x __attribute__( ( __packed__ ) )
#endif

/*
 *  LHCb namespace declaration
 */
namespace LHCb {

  /** @class OnlineRunInfo OnlineRunInfo.h MDF/OnlineRunInfo.h
   *
   * Basic run information from ODIN bank.
   */
  ONLINERUNINFO_ALIGN( struct ) OnlineRunInfo {
    unsigned int  Run;
    short int     EventType;
    short int     CalibrationStep;
    unsigned int  Orbit;
    long long int L0ID;
    long long int GPSTime;
    unsigned      detStatus : 24;
    unsigned      errors : 8;
    unsigned      bunchID : 12;
    unsigned      TAEWindow : 3;
    unsigned      pad0 : 1;
    unsigned      triggerType : 3;
    unsigned      readoutType : 2;
    unsigned      forceBit : 1;
    unsigned      bxType : 2;
    unsigned      bunchCurrent : 8;
    unsigned int  TCK;
  };
} // end namespace LHCb
#undef ONLINERUNINFO_ALIGN
#ifdef _WIN32
#  pragma pack( pop, onlineruninfo_aligment )
#endif

#endif // MDF_ONLINERUNINFO_H
