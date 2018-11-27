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
#ifndef VELORAWBANKDECODER_H 
#define VELORAWBANKDECODER_H 1

#include "SiDAQ/SiRawBankDecoder.h"
#include "VeloClusterWord.h"


/** @class VeloRawBankDecoder VeloRawBankDecoder.h
 *  
 *  Decoder for raw Velo banks
 *
 *  This is a simple type wrapper for a template
 *  instantiation of a SiRawBankDecoder with
 *  VeloCLusterWord.
 *
 *  @see VeloClusterWord
 *  @see SiRawBankDecoder
 * 
 *  @author Kurt Rinnert
 *  @date   2006-02-08
 */
typedef SiRawBankDecoder<VeloClusterWord> VeloRawBankDecoder;

#endif // VELORAWBANKDECODER_H
