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
#ifndef SRC_CONDDBDQSCANNER_H
#define SRC_CONDDBDQSCANNER_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/SmartIF.h"

// Implemented interfaces
#include "Kernel/IDQScanner.h" // IDQScanner

class IConverter;

/** Basic implementation of an IDQScanner based on the Conditions Database.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
class CondDBDQScanner: public extends<GaudiTool, IDQScanner> {
public:
  using extends::extends;

  /// Scan all the Data Quality flags in the give time range in the CondDB.
  /// @return merged list of DQ flags
  IDQFilter::FlagsType scan(const Gaudi::Time& since, const Gaudi::Time& until) const override;

  StatusCode initialize() override;
  StatusCode finalize() override;

private:
  Gaudi::Property<std::string> m_condPath{this, "ConditionPath", "/Conditions/DQ/Flags",
    "Path in the Conditions Database where to find the Data Quality condition."};
  Gaudi::Property<std::string> m_converterName{this, "Converter", "DetectorPersistencySvc",
    "Service implementing the IConverter interface."};

  /// ICondDBReader instance.
  SmartIF<IConverter> m_converter;
};

#endif // SRC_CONDDBDQSCANNER_H
