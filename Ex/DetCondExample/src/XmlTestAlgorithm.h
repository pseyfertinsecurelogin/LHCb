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
// $Id: XmlTestAlgorithm.h,v 1.2 2005-07-14 15:14:17 marcocle Exp $
#ifndef DETCONDEXAMPLE_XMLTESTALGORITHM_H
#define DETCONDEXAMPLE_XMLTESTALGORITHM_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

///---------------------------------------------------------------------------
/** @class XmlTestAlgorithm XmlTestAlgorithm.h DetCondExample/XmlTestAlgorithm.h

    Example of an algorithm retrieving condition data stored in XML files.

    Pointers to relevant DataObjects are retrieved at every new event.
    The retrieved objects are explicitly updated to ensure they are valid.

    @author Andrea Valassi
    @date December 2001
*///--------------------------------------------------------------------------

class XmlTestAlgorithm : public GaudiAlgorithm {

 public:

  /// Constructor.
  XmlTestAlgorithm( const std::string& name, ISvcLocator* pSvcLocator );

  // Algorithm standard methods.
  StatusCode execute() override;

};

#endif    // DETCONDEXAMPLE_XMLTESTALGORITHM_H
