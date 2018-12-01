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
#ifndef UNPACKERBASEALG_H
#define UNPACKERBASEALG_H 1

#include "GaudiAlg/GaudiAlgorithm.h"

namespace DataPacking
{

  /** @class Unpack UnpackerBaseAlg.h
   *
   *  Templated base algorithm for all unpacking algorithms
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-14
   */

  template< class PACKER >
  class Unpack : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    Unpack( const std::string& name, ISvcLocator* pSvcLocator );

    StatusCode initialize() override; ///< Algorithm initialize
    StatusCode execute() override;    ///< Algorithm execution

  private:

    std::string m_inputName;  ///< Input location
    std::string m_outputName; ///< Output location
    bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing

    /// Packer object
    const PACKER m_packer{ this };

  };

}

#endif // UNPACKERBASEALG_H
