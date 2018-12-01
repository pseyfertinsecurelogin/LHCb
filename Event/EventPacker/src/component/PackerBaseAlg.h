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
#ifndef PACKERBASEALG_H
#define PACKERBASEALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

namespace DataPacking
{

  /** @class Pack PackerBaseAlg.h
   *
   *  Templated base algorithm for all packing algorithms
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-14
   */

  template< class PACKER >
  class Pack : public GaudiAlgorithm
  {

  public:

    /// Standard constructor
    Pack( const std::string& name, ISvcLocator* pSvcLocator );

    StatusCode initialize() override; ///< Algorithm initialize
    StatusCode execute() override;    ///< Algorithm execution

  private:

    std::string m_inputName;  ///< Input location
    std::string m_outputName; ///< Output location
    unsigned short int m_version; ///< Packing version number
    bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
    bool m_deleteInput;       ///< Flag to turn on the removal of the input data after packing
    bool m_enableCheck;       ///< Flag to turn on automatic unpacking and checking of the output post-packing
    bool m_clearRegistry;     ///< Flag to turn on the clearing of the registry if the input data is not kept

    /// Packer object
    const PACKER m_packer{ this };

  };

}

#endif // PACKERBASEALG_H
