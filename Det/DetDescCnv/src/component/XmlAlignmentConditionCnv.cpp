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
// $Id: XmlAlignmentConditionCnv.cpp,v 1.2 2006-12-14 13:14:09 ranjard Exp $
// Include files 
#ifndef XMLALIGNMENTCONDITION_H 
#define XMLALIGNMENTCONDITION_H 1

#ifdef __INTEL_COMPILER         // Disable ICC remark from Math headers
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif

// Include files
#include "DetDesc/AlignmentCondition.h"
#include "DetDescCnv/XmlUserConditionCnv.h"

typedef  XmlUserConditionCnv<AlignmentCondition>  XmlAlignmentConditionCnv;
DECLARE_CONVERTER( XmlAlignmentConditionCnv )

#endif // XMLALIGNMENTCONDITION_H
