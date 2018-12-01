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
// ============================================================================
#ifndef CALODET_CALODETTESTALGORITHM_H
#define CALODET_CALODETTESTALGORITHM_H 1
// Include files
// from STL
#include <string>
// from CaloKernel
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloDet
#include "CaloDet/DeCalorimeter.h"

/** @class CaloDetTestAlgorithm CaloDetTestAlgorithm.h
 *
 *  Simple Test         Algorithm
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   14/12/2001
 */

class CaloDetTestAlgorithm : public GaudiAlgorithm
{
public:
  /** standard algorithm initialization
   *  @return status code
   */
  StatusCode initialize() override;

  /** Standard constructor
   *  @param   name   algorithm name
   *  @param   svcloc pointer to service locator
   */
  CaloDetTestAlgorithm( const std::string& name   ,
                        ISvcLocator*       svcloc );

private:
  std::string m_DetData;
};

// ============================================================================
// The End
// ============================================================================
#endif // CaloDetTestAlgorithm_H
// ============================================================================
