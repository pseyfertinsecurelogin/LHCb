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
// $Id: XmlDeVeloRTypeCnv.cpp,v 1.2 2006-11-30 07:57:41 cattanem Exp $

/// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
/// VeloDet
#include "VeloDet/DeVeloRType.h"

/// =======================================================================
/// Instantiation of a static factory class used by clients to create
/// instances of this service
/// =======================================================================
typedef XmlUserDetElemCnv<DeVeloRType> XmlDeVeloRTypeCnv;
DECLARE_CONVERTER( XmlDeVeloRTypeCnv )
