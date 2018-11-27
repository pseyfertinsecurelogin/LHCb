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
// $Id: VeloDecodeConf.h,v 1.3 2008-04-29 11:06:21 szumlat Exp $
#ifndef VELODECODECONF_H
#define VELODECODECONF_H 1

// Include files

/** @namespace VeloDecodeConf VeloDecodeConf.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-03-23
 */

namespace VeloTELL1
{
  // general parameters describing layout of the data section
  // sections:
  constexpr int SectionBShift=1;
  constexpr int SectionCShift=2;
  constexpr int SectionsPerBlock=3;
  constexpr int EventInfoLength=8;
  constexpr int NumberOfALinks=64;
  constexpr int ALL_ALINKS=72;
  constexpr int PPFPGAPedBlock=192;
  constexpr int PPFPGADataSection=216;
  // parameters related to organization of the sections within data stream
  // blocks:
  constexpr int NumberOfPPFPGA=4;
  constexpr int PPFPGADataBlock=224;
  // banks types; BankTypes:
  constexpr int VeloFull=18;
  constexpr int VeloError=29;
  constexpr int VeloPedestal=30;
  constexpr int VeloHeader=40;   // this is not a real bank!
  constexpr int VeloEvtInfo=50;  // added for the sake of simplicity during decoding
  // details related to organization of the words within the data section
  // Decoding:
  constexpr int ALinkPerDataStream=2;
  constexpr int DataStreamPerSection=3;
  constexpr int ADCHeaders=4;
  constexpr int ADCShift=10;
  constexpr int CHANNELS=32;
  constexpr int ALinkShift=36;
  constexpr int ADCMask=0x000003ff;
  // entry dedicated to cope with asymmetric section C
  // emptySection:
  constexpr int emptyField=2;
  // error bank; errorBank:
  constexpr int NextMarker=1;
  constexpr int FirstMarkerLocation=4;
  constexpr int EmptyBankSize=28;
  constexpr int FullBankSize=52;
  // errorBankJumps -  distance in 32 bit words to next PPFPGA section
  // in current Error Bank
  // errorBankJumps:
  constexpr int NextBankIfEmptyJump=2;
  constexpr int BankJump=8;
  constexpr int PPFPGASectionJump=13;
  // pedSubConsts:
  constexpr int BIT_PERFECT_SUBTRACTOR=0;
  constexpr int FAST_SUBTRACTOR=1;
  constexpr int CONVERGENCE_LIMIT=6500;    // number of events needed for ped subtractor to
  constexpr int FAST_CONVERGENCE_LIMIT=200; // get stable value for pedestal
  // numbers of bits in ProcessInfo field in Event Info block
  // processing:
  constexpr int PED_SUBTRACTION=2;
  constexpr int COMMON_MODE_SUBTRACTION=3;
  constexpr int BIT_LIMIT=4;
  constexpr int FIR_FILTER=6;
  constexpr int CHANNEL_REORDERING=7;
  constexpr int CLUSTER_MAKER=8;
  constexpr int MEAN_COMMON_MODE_SUBTRACTION=9;
  // dataTypes:
  constexpr int SIM_DATA=0;
  constexpr int REAL_DATA=1;
  constexpr int DEV_DATA=2;
  //
  // const from reordering (facilitate using of the convertChannel tool)
  // modes:
  constexpr int NO_REORDERING=0;
  constexpr int PHI_SENSOR=1;
  constexpr int R_SENSOR=2;
  constexpr int PILE_UP=3;
  //
  // these parameters govern of the reordering algorithm behaviour
  // first one allows to calculate channel position before reordering
  // using it's new position; second one has analogous meaning
  // directions:
  constexpr int NEW_TO_OLD_POSITION=0;
  constexpr int OLD_TO_NEW_POSITION=1;
  // number of distinc stages for digitalization; 10 bit width band
  // digiDyn:
  constexpr int MAX_ADC_8_BIT=127;
  constexpr int MIN_ADC_8_BIT=128;    // use it as -MIN_ADC_8_BIT!!
  constexpr int MAX_ADC_9_BIT=255;
  constexpr int MIN_ADC_9_BIT=256;
  constexpr int MAX_ADC_10_BIT=511;
  constexpr int MIN_ADC_10_BIT=512;
  constexpr int DIGIT_MAX_COUNT=1023;
  //  clusterization:
  constexpr int DUMMY_STRIPS=64;
  constexpr int STRIPS_IN_PROC_CHANNEL=64;  /// number of strips per PPFPGA proc. channel
  constexpr int STRIPS_IN_PPFPGA=512;
  constexpr int SENSOR_CHANNELS=2048;
  // test run for VeloTELL1Algorithms
  // testing:
  constexpr int TEST_RUN=0;
  constexpr int PROCESSING_RUN=1;
  // saturation modes
  //saturation:
  constexpr int CUT_MSB=0;
  constexpr int CUT_MIDDLE=1;
  constexpr int CUT_LSB=2;
}
#endif // VELODECODECONF_H
//
