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
#ifndef UNPACKRECVERTEX_H
#define UNPACKRECVERTEX_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/PackedRecVertex.h"
#include "Event/RecVertex.h"

/** @brief Unpack a PackedRecVertex container to RecVertices.
 *
 *  @author Olivier Callot
 *  @date   2008-11-17
 */
class UnpackRecVertex : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  StatusCode execute() override;

private:
  DataObjectReadHandle<LHCb::PackedRecVertices> m_packedVertices{this, "InputName",
                                                                 LHCb::PackedRecVertexLocation::Primary};
  DataObjectWriteHandle<LHCb::RecVertices>      m_vertices{this, "OutputName", LHCb::RecVertexLocation::Primary};
  Gaudi::Property<bool>                         m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                       "Flag to turn on the creation of output, even when input is missing"};
};

#endif // UNPACKRECVERTEX_H
