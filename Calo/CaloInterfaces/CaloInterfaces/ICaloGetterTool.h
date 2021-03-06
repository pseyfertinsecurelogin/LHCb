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
#ifndef ICALOGETTERTOOL_H
#define ICALOGETTERTOOL_H 1

// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloHypo.h"
struct ICaloDataProvider;

/** @class ICaloGetterTool ICaloGetterTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-04-17
 */
struct ICaloGetterTool : extend_interfaces<IAlgTool> {
  // Return the interface ID
  DeclareInterfaceID( ICaloGetterTool, 4, 0 );
  virtual void update() = 0;

  virtual void addToDigits( const std::string& loc, bool clear )   = 0;
  virtual void addToClusters( const std::string& loc, bool clear ) = 0;
  virtual void addToHypos( const std::string& loc, bool clear )    = 0;

  virtual LHCb::CaloDigits*   digits( const std::string& loc )   = 0;
  virtual LHCb::CaloClusters* clusters( const std::string& loc ) = 0;
  virtual LHCb::CaloHypos*    hypos( const std::string& loc )    = 0;
  virtual int                 detectorsMask()                    = 0;

  virtual bool               hasData( const std::string& det )  = 0;
  virtual ICaloDataProvider* provider( const std::string& det ) = 0;
};
#endif // ICALOGETTERTOOL_H
