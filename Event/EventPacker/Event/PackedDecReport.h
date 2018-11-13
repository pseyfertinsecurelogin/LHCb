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
#ifndef EVENT_PACKEDDECREPORT_H
#define EVENT_PACKEDDECREPORT_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include <string>
#include <vector>

namespace LHCb
{

  constexpr CLID CLID_PackedDecReport = 1559;

  // Namespace for locations in TDS
  namespace PackedDecReportLocation
  {
    inline const std::string Default = "pStrip/Phys/DecReports";
  }

  /** @class PackedDecReport PackedDecReport.h Event/PackedDecReport.h
   *
   *  Packed DecReport
   *
   *  @author Olivier Callot
   *  @date   2012-01-20
   */
  class PackedDecReport : public DataObject
  {

  public:

    /// Standard constructor
    PackedDecReport( ) { m_data.reserve(1000); }

  public:

    const CLID& clID()        const override { return PackedDecReport::classID(); }
    static  const CLID& classID()           { return CLID_PackedDecReport;       }

  public:

    std::vector<unsigned int>& reports()             { return m_data; }
    const std::vector<unsigned int>& reports() const { return m_data; }

    void setConfiguredTCK( unsigned int value )      { m_configuredTCK = value; }
    unsigned int configuredTCK() const               { return m_configuredTCK; }

  private:

    std::vector<unsigned int> m_data;
    unsigned int m_configuredTCK{0};

  };

}
#endif // EVENT_PACKEDDECREPORT_H
