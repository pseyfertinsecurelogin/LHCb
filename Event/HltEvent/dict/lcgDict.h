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
// $Id: lcgDict.h,v 1.5 2009-06-11 12:03:13 jpalac Exp $
#ifndef DICT_LCGDICT_H
#define DICT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// begin include files
#include "Event/HltDecReports.h"
#include "Event/HltObjectSummary.h"
//#include "Event/HltVertexReports.h"
// end include files

namespace {

  struct _Instantiations {
    // begin instantiations
    std::pair<std::string, LHCb::HltDecReport>                                  _dr0;
    std::vector<std::pair<std::string, LHCb::HltDecReport>>                     _dr1;
    GaudiUtils::VectorMap<std::string, LHCb::HltDecReport>                      _dr2;
    std::pair<std::string, LHCb::HltObjectSummary>                              _ii0;
    std::vector<std::pair<std::string, LHCb::HltObjectSummary>>                 _ii1;
    GaudiUtils::VectorMap<std::string, LHCb::HltObjectSummary>                  _ii2;
    std::vector<const LHCb::HltObjectSummary*>                                  _ii3;
    std::vector<LHCb::HltObjectSummary*>                                        _ii4;
    LHCb::HltObjectSummary::Info                                                _ii5;
    GaudiUtils::VectorMap<std::string, LHCb::HltVertexReports::HltVertexReport> _vr0;
    // end instantiations
  };
} // namespace

#endif // DICT_LCGDICT_H
