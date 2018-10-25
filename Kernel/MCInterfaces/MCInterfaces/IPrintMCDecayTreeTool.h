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
#ifndef MCINTERFACES_IMCPRINTDECAYTREETOOL_H
#define MCINTERFACES_IMCPRINTDECAYTREETOOL_H 1

// Include files
// from STL
#include <vector>
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

#include "Event/MCParticle.h"


/** @class IPrintMCDecayTreeTool IPrintMCDecayTreeTool.h DaVinciMCTools/IPrintMCDecayTreeTool.h
 *  
 *
 *  Interface for the printing of decay trees of MCParticles.
 *  Taken from the original IDebugTool from Olivier Dormond.
 *  
 *  @todo figure out exactly what each method is supposed to do 
 *        and clarify the doxygen!!!!
 *  @author Juan Palacios juancho@nikhef.nl
 *  @date   09/10/2007
 */
struct IPrintMCDecayTreeTool : extend_interfaces<IAlgTool> {

  /// Retrieve interface ID
  DeclareInterfaceID(IPrintMCDecayTreeTool, 2, 0);

  /// Print decay tree for a given MC particle up to a depth maxDepth.
  /// Default value of -1 is what exactly???
  virtual void printTree( const LHCb::MCParticle* mother, 
                          int maxDepth = -1 ) const = 0;

  /// Print all the MC particles leading to this one.
  virtual void printAncestor( const LHCb::MCParticle* child ) const = 0;

  /// Print all the MCParticles in a vector as a tree.
  /// Top level particles are those without parent.
  virtual void printAsTree( const LHCb::MCParticle::ConstVector& event ) const = 0;
  /// Print all the MCParticles in a keyed container as a tree.
  /// Top level particles are those without parent.
  virtual void printAsTree( const LHCb::MCParticles& event ) const = 0;

  /// Print all the MCParticles in a vector as a flat list.
  /// Top level particles are those without parent.
  virtual void printAsList( const LHCb::MCParticle::ConstVector& event ) const = 0;

  /// Print all the MCParticles in a keyed container as a flat list.
  /// Top level particles are those without parent.
  virtual void printAsList( const LHCb::MCParticles& event ) const = 0;

};
#endif // MCINTERFACES_IPRINTDECAYTREETOOL_H
