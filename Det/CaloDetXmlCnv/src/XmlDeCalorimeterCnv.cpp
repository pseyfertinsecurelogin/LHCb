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
// include
// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// CaloDet
#include "CaloDet/DeCalorimeter.h"

// ============================================================================
/** @file XmlDeCalorimeterCnv.cpp
 *
 * Instantiation of a static factory class used by clients to create
 * instances of Xml converter for class DeCalorimeter
 * @see DeCalorimeter
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================

/// converter type
typedef XmlUserDetElemCnv<DeCalorimeter> XmlDeCalorimeterCnv;
DECLARE_CONVERTER( XmlDeCalorimeterCnv )

// ============================================================================
// The End
// ============================================================================
