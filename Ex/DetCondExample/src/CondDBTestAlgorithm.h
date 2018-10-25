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
#ifndef CONDDBTESTALGORITHM_H
#define CONDDBTESTALGORITHM_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Forward declarations
class DataObject;
struct Condition;
class IDetDataSvc;
class AlignmentCondition;

/** @class CondDBTestAlgorithm CondDBTestAlgorithm.h
 *
 *  Example of an algorithm retrieving condition data stored in the CondDB.
 *
 *  Pointers to relevant DataObjects are retrieved at every new event.
 *  The retrieved objects are explicitly updated to ensure they are valid.
 *
 *  @author Andrea Valassi
 *  @date December 2001
 *
 *  @author Marco Clemencic
 *  @date   2005-06-23
 */
class CondDBTestAlgorithm : public GaudiAlgorithm {
public:
  /// Standard constructor
  CondDBTestAlgorithm( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~CondDBTestAlgorithm( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  /// Dump interesting information about a DataObject
  StatusCode i_analyse( DataObject* pObj );

  /// Internal member function used with the UpdateManagerSvc
  StatusCode i_updateCacheLHCb();
  /// Internal member function used with the UpdateManagerSvc
  StatusCode i_updateCacheHcal();
  /// Internal member function used with the UpdateManagerSvc
  StatusCode i_updateCache();

  /// Cached pointer to Condition
  Condition *m_LHCb_cond;
  /// Cached pointer to Condition
  Condition *m_Hcal_cond;
  /// Cached pointer to TabulatedProperty
  TabulatedProperty *m_TabProp;
  /// Cached pointers to AlignmentCondition
  AlignmentCondition *m_m01, *m_m12;

  double m_LHCb_temp;
  double m_Hcal_temp;
  double m_avg_temp;

  size_t m_evtCount;

  IDetDataSvc *m_dds;

  bool m_direct_mapping_test;

};
#endif // CONDDBTESTALGORITHM_H
