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
// $Id: RawEventPrintout.h,v 1.5 2008-02-05 16:44:18 frankb Exp $
//====================================================================
//  RawEventPrintout.h
//--------------------------------------------------------------------
//
//====================================================================
#ifndef MDF_RAWEVENTPRINTOUT_H
#define MDF_RAWEVENTPRINTOUT_H 1

#include <string>
/*
 *    LHCb namespace declaration
 */
namespace LHCb  {

  // Forward declarations:
  class RawBank;
  class RawEvent;

  /** @class RawEventPrintout RawEventPrintout.h MDF/RawEventPrintout.h
    *
    */
  class RawEventPrintout  {
  public:
    static std::string bankHeader(const RawBank* r);
    static std::string bankType(const RawBank* r);
    static std::string bankType(int i);
  };
}   // End namespace LHCb
#endif // MDF_RAWEVENTPRINTOUT_H
