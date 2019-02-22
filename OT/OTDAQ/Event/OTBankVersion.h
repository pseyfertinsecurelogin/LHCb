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
#ifndef EVENT_OTBANKVERSION_H
#define EVENT_OTBANKVERSION_H 1

// Include files

/** @file OTBankVersion.h Event/OTBankVersion.h
 *  Version numbers of OT RawBuffer bank encoding
 *
 *  @author Marco Cattaneo
 *  @date   2004-10-26
 */
namespace OTBankVersion {
  enum versions { DC04 = 0, v1 = 1, v2 = 2, DC06 = v2, SIM = 63, v3 = 3, DEFAULT = v3, UNDEFINED = 255 };
}
#endif // EVENT_OTBANKVERSION_H
