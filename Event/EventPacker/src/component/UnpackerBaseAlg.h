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

namespace DataPacking {

  /** @class Unpack UnpackerBaseAlg.h
   *
   *  Templated base algorithm for all unpacking algorithms
   *
   *  @author Christopher Rob Jones
   *  @date   2009-10-14
   */

  template <class PACKER>
  class Unpack : public GaudiAlgorithm {

  public:
    using GaudiAlgorithm::GaudiAlgorithm;
    StatusCode initialize() override; ///< Algorithm initialize
    StatusCode execute() override;    ///< Algorithm execution

  private:
    DataObjectReadHandle<typename PACKER::PackedDataVector> m_packedObjects{this, "InputName",
                                                                            PACKER::packedLocation()};
    DataObjectWriteHandle<typename PACKER::DataVector>      m_objects{this, "OutputName", PACKER::unpackedLocation()};
    Gaudi::Property<bool>                                   m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                         "Flag to turn on the creation of output, even when input is missing"};

    /// Packer object
    const PACKER m_packer{this};
  };

} // namespace DataPacking

#endif // UNPACKERBASEALG_H
