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
//$Header: /afs/cern.ch/project/cvs/reps/lhcb/Ex/DetCondExample/src/DumpDetectorStore.h,v 1.3 2006-08-31 13:53:44 marcocle Exp $
#ifndef DETCONDEXAMPLE_DUMPDETECTORSTORE_H
#define DETCONDEXAMPLE_DUMPDETECTORSTORE_H 1

// Base class
#include "GaudiAlg/GaudiAlgorithm.h"

///---------------------------------------------------------------------------
/** @class DumpDetectorStore DumpDetectorStore.h DetCondExample/DumpDetectorStore.h

    Simple algorithm to dump the detector data store.

    @author Andrea Valassi
    @date August 2001
*///--------------------------------------------------------------------------

class DumpDetectorStore : public GaudiAlgorithm {

 public:

  /// Constructor
  DumpDetectorStore ( const std::string& name, ISvcLocator* pSvcLocator );

  /// Only finalize is implemented (GaudiAlgorithm::initialize() is enough).
  StatusCode execute() override;
  StatusCode finalize() override;

private:
  bool m_dumpConds;

};

#endif    // DETCONDEXAMPLE_DUMPDETECTORSTORE_H



