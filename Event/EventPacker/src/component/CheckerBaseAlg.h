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
#ifndef CHECKERBASEALG_H
#define CHECKERBASEALG_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

namespace DataPacking
{

  /** @class Check CheckerBaseAlg.h
   *
   *  Templated base algorithm for checking data packing
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-14
   */

  template< class PACKER >
  class Check : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    Check( const std::string& name, ISvcLocator* pSvcLocator );

    StatusCode execute() override; ///< Algorithm execution

  private:

    std::string m_dataA;  ///< Data location
    std::string m_dataB;  ///< Data location
    const PACKER m_packer{this};  ///< Packer

  };

}

#endif // CHECKERBASEALG_H
