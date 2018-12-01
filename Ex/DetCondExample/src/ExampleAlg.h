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
#ifndef EXAMPLEALG_H
#define EXAMPLEALG_H 1

#include "DetDesc/Condition.h"
#include "DetDesc/AlignmentCondition.h"

#include "GaudiAlg/GaudiAlgorithm.h"


/** @class ExampleAlg ExampleAlg.h
 *
 *  Example of an algorithm using conditions.
 *
 *  @author Marco Clemencic
 *  @date   2006-01-22
 */
class ExampleAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  ExampleAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~ExampleAlg( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// Cached pointer to Condition
  Condition *m_myCond1;
  /// Cached pointer to Condition
  Condition *m_myCond2;
  /// Cached pointer to AlignmentCondition
  AlignmentCondition *m_myAlignment;

  /// Function extracting data from Cond1
  StatusCode i_cacheCondition1Data();
  /// Function extracting data from Cond2
  StatusCode i_cacheCondition2Data();
  /// Function extracting data from Alignment
  StatusCode i_cacheAlignmentData();

  /// Value taken from Cond1
  double           m_cond1;
  /// Values taken from Cond2
  std::vector<int> m_cond2;

};
#endif // EXAMPLEALG_H
