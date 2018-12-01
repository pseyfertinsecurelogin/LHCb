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
#ifndef MCINTERFACES_IMCRECONSTRUCTED_H 
#define MCINTERFACES_IMCRECONSTRUCTED_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Event Model
namespace LHCb
{
  class MCParticle;
}


/** @class IMCReconstructed IMCReconstructed.h MCInterfaces/IMCReconstructed.h
 *   
 *
 *  @author Christian Jacoby
 *  @date   2004-03-08
 */
struct IMCReconstructed : extend_interfaces<IAlgTool> {

  /// Reconstructed categories
  enum RecCategory
  {
    NoClassification=-1,  ///< No MC classification possible (e.g. NO MC)
    NotReconstructed=0,   ///< Not reconstructed
    ChargedLong,          ///< Reconstructed as a Long charged track
    ChargedDownstream,    ///< Reconstructed as a Downstream charged track
    ChargedUpstream,      ///< Reconstructed as an Upstream charged track
    ChargedTtrack,        ///< Reconstructed as a T charged track
    ChargedVelo,          ///< Reconstructed as a VELO charged track
    ChargedVeloR,         ///< Reconstructed as a 2D Velo track
    ChargedMuon,          ///< Reconstructed as a Muon track
    Neutral=50,           ///< Reconstructed as a Neutral particle 
    NeutralMergedPi0,     ///< Reconstructed as a merged pi0
    CatUnknown            ///< Reconstructed as a type that doesn't fit other categories
  };

  /// Convert type enum to string
  static std::string text( const RecCategory cat );

  // Return the interface ID
  DeclareInterfaceID(IMCReconstructed, 2, 0 );

  /// check if MCParticle is reconstructed
  virtual IMCReconstructed::RecCategory reconstructed( const LHCb::MCParticle * ) = 0;

};


//-----------------------------------------------------------------------------
// Implementation of enum printout
//-----------------------------------------------------------------------------

inline std::string IMCReconstructed::text( const IMCReconstructed::RecCategory cat )
{
  switch ( cat )
  {
    case IMCReconstructed::NoClassification  : return "No MC classification";
    case IMCReconstructed::NotReconstructed  : return "Not reconstructed";
    case IMCReconstructed::ChargedLong       : return "Charged Long Particle";
    case IMCReconstructed::ChargedDownstream : return "Charged Downstream Particle";
    case IMCReconstructed::ChargedUpstream   : return "Charged Upstream Particle";
    case IMCReconstructed::ChargedTtrack     : return "Charged Ttrack Particle";
    case IMCReconstructed::ChargedVelo       : return "Charged VELO Particle";
    case IMCReconstructed::ChargedVeloR      : return "Charged 2D Velo track";
    case IMCReconstructed::ChargedMuon       : return "Charged Muon track";
    case IMCReconstructed::Neutral           : return "Neutral Particle ";
    case IMCReconstructed::NeutralMergedPi0  : return "Neutral Merged Pi0";
    case IMCReconstructed::CatUnknown        :
    default                                  : return "Unknown Type";
  }
}

#endif // MCINTERFACES_IMCRECONSTRUCTED_H
