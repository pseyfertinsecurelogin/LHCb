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
// $Id: MuonNamespace.h,v 1.1 2007-06-11 08:37:06 asatta Exp $
#ifndef MUONDET_MUONNAMESPACE_H 
#define MUONDET_MUONNAMESPACE_H 1
#ifdef __INTEL_COMPILER        // Disable ICC remark
  #pragma warning(disable:177) // variable was declared but never referenced
#endif
#include <string>
namespace DeMuonLocation{
  static const std::string& Default="/dd/Structure/LHCb/DownstreamRegion/Muon";
  static const std::string& Cabling="/dd/Conditions/ReadoutConf/Muon/Cabling";
}
#endif // MUONDET_MUONNAMESPACE_H
