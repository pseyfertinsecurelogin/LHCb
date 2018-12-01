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
#ifndef TOOLS_IMCDECAYFINDER_H 
#define TOOLS_IMCDECAYFINDER_H 1

// Include files
// from STL
#include <string>
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/MCParticle.h"


/** @class IMCDecayFinder IMCDecayFinder.h DaVinciTools/IMCDecayFinder.h
 *
 *  Please see the file MCDecayFinder.h
 *  in the src directory of package Phys/DaVinciTools.
 *
 *  @author Olivier Dormond
 *  @date   12/03/2002
 */
struct IMCDecayFinder : extend_interfaces<IAlgTool> {

  /// Retrieve interface ID
  DeclareInterfaceID( IMCDecayFinder, 5, 0);

  /// Get/Set the decay string to find
  virtual std::string decay( void ) const = 0;
  virtual StatusCode setDecay( std::string decay ) = 0;

  virtual std::string revert( void ) const = 0;

  /// Does the described decay exists in the event?
  virtual bool hasDecay(  const LHCb::MCParticle::ConstVector& event ) const = 0;
  virtual bool findDecay( const LHCb::MCParticle::ConstVector& event,
                          const LHCb::MCParticle*& previous_result ) const = 0;

  virtual bool hasDecay(  const LHCb::MCParticle::Container& event ) const = 0;
  virtual bool findDecay( const LHCb::MCParticle::Container& event,
                          const LHCb::MCParticle*& previous_result ) const = 0;

  virtual bool hasDecay( void ) const = 0;
  virtual bool findDecay( const LHCb::MCParticle*& previous_result ) const = 0;

  virtual void descendants( const LHCb::MCParticle *head,
                            LHCb::MCParticle::ConstVector& result,
                            bool leaf=false ) const = 0;
  virtual void decayMembers( const LHCb::MCParticle* head,
                             LHCb::MCParticle::ConstVector& members ) const = 0;
  virtual void decaySubTrees( const LHCb::MCParticle* head,
                              std::vector<std::pair<const LHCb::MCParticle*,
                                                    LHCb::MCParticle::ConstVector >
                                         > & subtrees ) const = 0;

};
#endif // TOOLS_IMCDECAYFINDER_H
