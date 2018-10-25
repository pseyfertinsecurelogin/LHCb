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
#ifndef L0CALOCOMPARE_H
#define L0CALOCOMPARE_H 1

// Include files
#include "CaloUtils/Calo2Dview.h"

// AIDA
#include "AIDA/IHistogram1D.h"

/** @class L0CaloCompare L0CaloCompare.h
 *
 *  Monitoring algorithm for the L0 Calorimeter trigger
 *  Scans the L0CaloCandidate and fills histograms.
 *
 */
class L0CaloCompare: public Calo2Dview
{

public:

  /// Standard constructor
  L0CaloCompare(const std::string& name, ISvcLocator* pSvcLocator );

  /// Standard destructor
  virtual ~L0CaloCompare( );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// Prints name of candidate
  std::string candidateTypeName( int type ) const ;

private:

  bool m_fullMonitoring ;

  AIDA::IHistogram1D*            m_histSpdMult_Comp ;
  AIDA::IHistogram1D*            m_histSumEt_Comp ;

  AIDA::IHistogram1D*            m_histTotalCount ;
  AIDA::IHistogram1D*            m_histErrorCount ;

  std::string              m_referenceDataSuffix ;
  std::string              m_checkDataSuffix ;

  std::vector< std::string >  m_mapCompareName ;
  std::vector< std::string >  m_mapCompareTitle ;
  std::vector< std::string >  m_mapAllName ;
  std::vector< std::string >  m_mapAllTitle ;
  std::vector< std::string >  m_errorCounterName ;
  std::vector< std::string >  m_errorCounterTitle ;

  std::vector<AIDA::IHistogram1D*>  m_errorCounterHisto ;

  std::set< unsigned int > m_idles ; ///< List of idle BCIds
  /// Vector of Idles BCIds (to set via job options)
  std::vector< unsigned int > m_idleBCIdVector ;

};
#endif // L0CALOCOMPARE_H
