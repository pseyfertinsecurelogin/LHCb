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
#ifndef SRC_CONDDBDQSCANNER_H
#define SRC_CONDDBDQSCANNER_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/SmartIF.h"

// Implemented interfaces
#include "Kernel/IDQScanner.h" // IDQScanner

class ICondDBReader;
class IConverter;

/** Basic implementation of an IDQScanner based on the Conditions Database.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
class CondDBDQScanner: public extends1<GaudiTool, IDQScanner> {
public:
  /// Standard constructor
  CondDBDQScanner(const std::string& type, const std::string& name, const IInterface* parent);
  virtual ~CondDBDQScanner(); ///< Destructor

  /// Scan all the Data Quality flags in the give time range in the CondDB.
  /// @return merged list of DQ flags
  IDQFilter::FlagsType scan(const Gaudi::Time& since, const Gaudi::Time& until) const override;

  StatusCode initialize() override; ///< Initialize the instance.
  StatusCode finalize() override; ///< Finalize the instance.

protected:
private:

  /// Path to the condition object containing the Data Quality flags.
  /// (property ConditionPath)
  std::string m_condPath;

  /// ICondDBReader implementation to use to access the Conditions Database.
  /// (property CondDBReader)
  std::string m_condDBReaderName;

  /// IConverter implementation (e.g. the persistency service) to use to convert
  /// the data to a Condition.
  /// (property Converter)
  std::string m_converterName;

  /// ICondDBReader instance.
  SmartIF<ICondDBReader> m_condDB;

  /// ICondDBReader instance.
  SmartIF<IConverter> m_converter;

};

#endif // SRC_CONDDBDQSCANNER_H
