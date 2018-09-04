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
#include <UTDAQ/UTReadoutTool.h>
#include <UTDAQ/UTClustersToRawBankAlg.h>
#include <UTDAQ/UTDigitsToUTTELL1Data.h>
#include <UTDAQ/UTErrorDecoding.h>
#include <UTDAQ/UTFullDecoding.h>
#include <UTDAQ/UTLayerSelector.h>
#include <UTDAQ/UTPedestalDecoding.h>
#include <UTDAQ/UTRawBankMonitor.h>
#include <UTDAQ/RawBankToUTClusterAlg.h>
#include <UTDAQ/RawBankToUTLiteClusterAlg.h>


DECLARE_COMPONENT( UTReadoutTool )
DECLARE_COMPONENT( UTClustersToRawBankAlg )
DECLARE_COMPONENT( UTDigitsToUTTELL1Data )
DECLARE_COMPONENT( UTErrorDecoding )
DECLARE_COMPONENT( UTFullDecoding )
DECLARE_COMPONENT( UTLayerSelector )
DECLARE_COMPONENT( UTPedestalDecoding )
DECLARE_COMPONENT( UTRawBankMonitor )
DECLARE_COMPONENT( RawBankToUTClusterAlg )
DECLARE_COMPONENT( RawBankToUTLiteClusterAlg )
