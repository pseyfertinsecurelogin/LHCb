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
// $Id: XmlDeSubSubCalorimeterCnv.cpp,v 1.1 2008-06-30 16:45:00 odescham Exp $ 
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.3  2006/12/14 12:51:16  ranjard
// v3r0 - InstallArea and new Plugins
//
// Revision 1.2  2003/06/16 13:36:27  sponce
// use new package DetDescCnv
//
// Revision 1.1.1.1  2002/03/28 13:32:34  ibelyaev
// Det/CaloDetXmlCnv: new package
//
// ============================================================================
// DetDesc 
#include  "DetDescCnv/XmlUserDetElemCnv.h"
// CaloDet 
#include  "CaloDet/DeSubSubCalorimeter.h"

// ============================================================================
/** @file XmlDeSubCalorimeterCnv.cpp
 * 
 * Instantiation of a static factory class used by clients to create
 * instances of Xml converter for class DeSubCalorimeter 
 * @see DeSubCalorimeter 
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================

/// converter type 
typedef  XmlUserDetElemCnv<DeSubSubCalorimeter>  XmlDeSubSubCalorimeterCnv;
DECLARE_CONVERTER( XmlDeSubSubCalorimeterCnv )
// ============================================================================
// The End 
// ============================================================================












