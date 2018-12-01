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
// $Id: LbAppInitFactory.cpp,v 1.1 2010-02-12 14:00:30 gcorti Exp $
// Include files 
#include "Kernel/LbAppInit.h"
#include "GaudiKernel/LoadFactoryEntries.h"

// Put LbAppInit base class factory in this library, to avoid having it in
// LHCbKernel component library. 
// It must be present otherwise the class cannot be instanciated
DECLARE_COMPONENT( LbAppInit )

