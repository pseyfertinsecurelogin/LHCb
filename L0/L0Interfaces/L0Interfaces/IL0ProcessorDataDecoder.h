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
// ============================================================================
#ifndef L0DU_IL0PROCESSORDATADECODER_H
#define L0DU_IL0PROCESSORDATADECODER_H

// STD & STL
#include <string>
#include <vector>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// from Event
#include "Event/L0ProcessorData.h"
#include "Event/L0DUBase.h"

/** @class IL0ProcessorDataDecoder IL0ProcessorDataDecoder.h
 *
 * Generic interface for tools decoding the container of L0Processor data
 *
 * ProcessorDataDecoder : L0DU/L0ProcessorDataDecoder
 *
 * [replace the obsolete IL0Candidate interface]
 *
 */

struct IL0ProcessorDataDecoder : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IL0ProcessorDataDecoder, 5, 0 );

  /*
   ** The main methods
   */
  virtual bool   setL0ProcessorData(const std::vector<std::string>& dataLocs )=0;
  virtual bool   setL0ProcessorData(const std::vector<LHCb::L0ProcessorDatas*>& datass ) = 0;
  virtual bool   setL0ProcessorData(const std::string& dataLoc )=0;
  virtual bool   setL0ProcessorData(LHCb::L0ProcessorDatas* datas ) = 0;
  virtual double value( const std::array<unsigned int,L0DUBase::Index::Size>& data,int bx=0) = 0;
  virtual unsigned long digit( const std::array<unsigned int,L0DUBase::Index::Size>& data,int bx=0) = 0;
  virtual std::vector<int> bxList( const std::array<unsigned int, L0DUBase::Index::Size>& data) = 0;
  virtual bool   isValid()=0;

};
#endif // L0DU_IL0PROCESSORDATADECODER_H
