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
#ifndef XMLDEHORIZRICHCNV_H
#define XMLDEHORIZRICHCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeHorizRich.h"

typedef XmlUserDetElemCnv<DeHorizRich> XmlDeHorizRichCnv;
DECLARE_CONVERTER( XmlDeHorizRichCnv )

#endif // XMLDEHORIZRICHCNV_H
