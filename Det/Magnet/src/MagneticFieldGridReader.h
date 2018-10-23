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

#pragma once

#include <vector>
#include <string>
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Vector3DTypes.h"

class IMessageSvc ;
struct GridQuadrant ;

namespace LHCb {
  class MagneticFieldGrid ;
}

class MagneticFieldGridReader
{
public:
  MagneticFieldGridReader(IMessageSvc& ) ;

  StatusCode readFiles( const std::vector<std::string>& filenames,
			LHCb::MagneticFieldGrid& grid ) const ;

  StatusCode readDC06File( const std::string& filename,
			   LHCb::MagneticFieldGrid& grid ) const ;

  void fillConstantField( const Gaudi::XYZVector& field ,
			  LHCb::MagneticFieldGrid& grid ) const ;
private:
  void fillGridFromQuadrants( GridQuadrant* quadrants,
			      LHCb::MagneticFieldGrid& grid ) const ;
  StatusCode readQuadrant( const std::string& filename,
			   GridQuadrant& quad ) const ;
private:
  mutable MsgStream m_msg ;
} ;

