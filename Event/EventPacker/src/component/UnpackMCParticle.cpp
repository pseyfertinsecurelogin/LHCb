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

#include "UnpackMCParticle.h"

#include "Event/MCParticle.h"
#include "Event/StandardPacker.h"
#include "Event/PackedMCParticle.h"

#include <algorithm>
#include <memory>

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackMCParticle
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( UnpackMCParticle )

StatusCode UnpackMCParticle::execute() 
{

  // CRJ : If packed data does not exist just return. Needed for packing of
  //     : spillover which is not neccessarily available for each event
  if ( !m_alwaysOutput.value() && !m_packedMCParticles.exist() )
    return StatusCode::SUCCESS;

  LHCb::PackedMCParticles* dst = m_packedMCParticles.getOrCreate();

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Size of PackedMCParticles = " << dst->mcParts().size() << endmsg;

  LHCb::MCParticles* newMCParticles = m_MCParticles.put(std::make_unique<LHCb::MCParticles>());

  StandardPacker pack(this);

  // Packing version
  const char pVer = dst->packingVersion();

  // random generator for private tests of flags. 
  //static std::default_random_engine gen;
  //static std::uniform_real_distribution<float> uniform(0,1);

  newMCParticles->reserve( dst->mcParts().size() );
  for ( const auto & src : dst->mcParts() )
  {

    LHCb::MCParticle * part = new LHCb::MCParticle( );
    newMCParticles->insert( part, src.key );

    const auto px = pack.energy( src.px );
    const auto py = pack.energy( src.py );
    const auto pz = pack.energy( src.pz );
    const auto mass = src.mass;
    const auto E = std::sqrt( (px*px) + (py*py) + (pz*pz) + (mass*mass) );
    part->setMomentum( Gaudi::LorentzVector( px, py, pz , E ) );

    part->setParticleID( LHCb::ParticleID(src.PID) );

    part->setFlags( src.flags );
    // for testing, randomly set 'fromSignal' to true 5% of the time.
    //part->setFromSignal( uniform(gen) > 0.95 );

    int hintID(0), key(0);
    if ( ( 0==pVer && pack.hintAndKey32( src.originVertex, dst, newMCParticles, hintID, key ) ) ||
         ( 0!=pVer && pack.hintAndKey64( src.originVertex, dst, newMCParticles, hintID, key ) ) )
    {
      SmartRef<LHCb::MCVertex> ref( newMCParticles, hintID, key );
      part->setOriginVertex( ref );
    }
    else { Error( "Corrupt MCParticle Origin MCVertex SmartRef detected" ).ignore(); }

    // List of processed refs, to check for duplicates
    std::vector<long long> processedRefs;
    processedRefs.reserve( src.endVertices.size() );

    // loop over refs and process
    for ( const auto& I : src.endVertices )
    {
      // Check for duplicates ...
      if ( std::none_of( processedRefs.begin(), processedRefs.end(),
                         [&I]( const auto& J ) { return I == J; } ) )
      {
        // save this packed ref to the list of those already processed.
        processedRefs.push_back(I);
        // Unpack the ref and save to the vertex
        hintID = key = 0;
        if ( ( 0==pVer && pack.hintAndKey32( I, dst, newMCParticles, hintID, key ) ) ||
             ( 0!=pVer && pack.hintAndKey64( I, dst, newMCParticles, hintID, key ) ) )
        {
          // Construct the smart ref
          SmartRef<LHCb::MCVertex> ref( newMCParticles, hintID, key );
          // save
          part->addToEndVertices( ref );
        }
        else { Error( "Corrupt MCParticle End MCVertex SmartRef detected" ).ignore(); }
      }
      else
      {
        Warning( "Found duplicate in packed MCParticle end vertices", 
                 StatusCode::SUCCESS ).ignore(); }
    }
  }
  
  return StatusCode::SUCCESS;
}

//=============================================================================
