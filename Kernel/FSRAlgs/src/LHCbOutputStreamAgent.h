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
//====================================================================
//	LHCbOutputStreamAgent.h
//--------------------------------------------------------------------
//
//	Package    :  (The LHCb PersistencySvc service)
//
//  Description: Definition the LHCbOutputStream Agent
//
//	Author     : M.Frank
//  Created    : 13/1/99
//	Changes    :
//
//====================================================================
#ifndef LHCB_OUTPUTSTREAMAGENT_H
#define LHCB_OUTPUTSTREAMAGENT_H

// Framework includes
#include "GaudiKernel/IDataStoreAgent.h"
#include "GaudiKernel/IRegistry.h"
// Foreward declarations
class LHCbOutputStream;

/** @name The LHCbOutputStreamAgent class.

  Data store Agent to traverse data store trees and select all
  items to be written to the output file.

  @author Markus Frank
 */
class LHCbOutputStreamAgent : virtual public IDataStoreAgent  {
  /// Reference to data writer
  LHCbOutputStream*  m_OutputStream;
public:
  /// Standard Constructor
  LHCbOutputStreamAgent(LHCbOutputStream* OutputStream);
  /// Standard Destructor
  virtual ~LHCbOutputStreamAgent();
  /// Analysis callback
  bool analyse(IRegistry* dir, int level) override;
};
#endif // LHCB_OUTPUTSTREAMAGENT_H
