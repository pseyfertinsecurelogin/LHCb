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
#include "Event/PrimaryVertex.h"
#include "Event/Track.h"
#include "Event/RecVertex.h"
#include "LHCbMath/BloomFilter.h"

namespace LHCb
{

  struct LHCbIDAdder
  {
    unsigned int operator()(const unsigned int& lhs, const LHCb::LHCbID& rhs)
    {
      return lhs + rhs.lhcbID() ;
    }
  };

  uint32_t PrimaryVertex::uniqueVeloSegmentID( const LHCb::Track& track )
  {
    // create a 'hash' from the velo IDs on the track
    const auto& lhcbids = track.lhcbIDs() ;
    const uint32_t* begin = reinterpret_cast<const uint32_t*>(&(*(lhcbids.begin()))) ;
    const uint32_t* end   = reinterpret_cast<const uint32_t*>(&(*(lhcbids.end()))) ;

    // // need a trick to find the last velo id: TT comes after velo. so
    // // we use lowerbound with the smallest TT ID. unfortunately need a
    // // number that is private in lhcbid:
    const int detectorTypeBits = 28 ;
    //const uint32_t* begin = reinterpret_cast<const uint32_t*>(&(*(lhcbids.begin()))) ;
    //const uint32_t* end   = reinterpret_cast<const uint32_t*>(&(*(lhcbids.end()))) ;

    // create a hash object (which performs the hash operations);
    // hashobj does not need memory, and construction is free (no CPU
    // required), so you can instantiate away...
    BloomFilterImpl::HashFNV1a<uint32_t, uint32_t> hashobj;

    //const uint32_t hash1 = hashobj(lhcbid);

    // hashing a second object on top of it
    //uint32_t hash2 = hashobj(lhcbid, hash1);

    // hashing a vector of objects - slightly ugly, since we need to get the
    // initial value of an "empty" hash from deep within the implementation...
    uint32_t hash = BloomFilterImpl::__doFNV1a<uint32_t>::hashinit;
    const uint32_t* it = begin ;
    const uint32_t maxval =  LHCb::LHCbID::channelIDtype::TT << detectorTypeBits ;
    while( it < end && *it < maxval ) { hash = hashobj(*it, hash); ++it; }
    return hash ;
  }

  //uint32_t PrimaryVertex::uniqueVeloSegmentIDVanya( const LHCb::Track& track )
  //{
  //  return LHCb::HashIDs::hashID ( &track , &LHCbID::isVelo ) ;
  //}

  PrimaryVertex::PrimaryVertex(const LHCb::RecVertex& recvertex, bool refit )
    : VertexBase( recvertex )
  {
    // reference position for all track states in vertex
    m_refZ = recvertex.position().z() ;

    // add all tracks. may want to use a 'zip' construction here ... in the future
    const auto N = recvertex.tracks().size() ;
    m_tracks.reserve(N) ;
    for(size_t i=0; i<N; ++i )
      if( recvertex.tracks()[i] )
        m_tracks.push_back( PrimaryVertexTrack(*(recvertex.tracks()[i]), recvertex.weights()[i],m_refZ) ) ;
    setNDoF( 2*m_tracks.size()-3 ) ;
    // sort the tracks by ID
    std::sort(m_tracks.begin(),m_tracks.end()) ;
    // compute the derivatives by adding everything up
    initCache() ;
    // refit if requested
    if(refit) fit() ;
  }

  void PrimaryVertex::initCache()
  {
    m_sumhalfD2Chi2DX2 = Gaudi::SymMatrix3x3() ;
    m_sumhalfDChi2DX  = Gaudi::Vector3() ;
    m_sumchi2 = 0 ;
    for( const auto& trk : m_tracks )
    {
      const auto w =  trk.weight() ;
      m_sumhalfD2Chi2DX2 += w * trk.halfD2Chi2DX2() ;
      m_sumhalfDChi2DX   += w * trk.halfDChi2DX() ;
      m_sumchi2 += w*trk.chi2() ;
    }
  }

  bool PrimaryVertex::fit()
  {
    // recompute position and chi2
    auto covariance = m_sumhalfD2Chi2DX2 ;
    const bool success = covariance.InvertChol() ;
    if ( success )
    {
      const auto delta = covariance * m_sumhalfDChi2DX ;
      const auto chi2  = m_sumchi2 - ROOT::Math::Dot(delta,m_sumhalfDChi2DX) ;
      setCovMatrix( covariance ) ;
      setPosition( Gaudi::XYZPoint(delta(0),delta(1),delta(2) + m_refZ) ) ;
      setChi2AndDoF( chi2,nDoF() ) ;
    }
    return success;
  }


  int PrimaryVertex::unbias( const std::vector<const LHCb::Track*>& tracksToRemove )
  {
    // subtract all the tracks from the cached chi2 sums
    int nremoved(0) ;
    for( const auto& track : tracksToRemove )
    {
      const auto it = find(uniqueVeloSegmentID(*track)) ;
      if ( it != m_tracks.end() )
      {
        auto& trk = *it ;
        const auto w =  trk.weight() ;
        if ( w > 0 )
        {
          ++nremoved ;
          m_sumhalfD2Chi2DX2 -= w * trk.halfD2Chi2DX2() ;
          m_sumhalfDChi2DX   -= w * trk.halfDChi2DX() ;
          m_sumchi2          -= w * trk.chi2() ;
          // rather than explicitly remove the track, we set the weight to zero.
          trk.setWeight(0) ;
        }
      }
    }

    if ( nremoved > 0 )
    {
      setNDoF( this->nDoF() - 2*nremoved ) ;
      const bool success = fit() ;
      if(!success) nremoved = -1 ;
    }
    return nremoved ;
  }

  int PrimaryVertex::unbias( const std::vector<const LHCb::Track*>& tracksToRemove,
                             LHCb::VertexBase& target ) const
  {
    // default: copy everything from the vertexbase
    target = *this ;

    // subtract all the tracks
    int nremoved(0) ;
    auto   W  = m_sumhalfD2Chi2DX2 ;
    auto   WX = m_sumhalfDChi2DX ;
    auto chi2 = m_sumchi2 ;
    for( const auto& track : tracksToRemove )
    {
      const auto it = find(uniqueVeloSegmentID(*track)) ;
      if( it != m_tracks.end() )
      {
        const auto& trk = *it ;
        const auto w =  trk.weight() ;
        if ( w > 0 )
        {
          W    -= it->weight() * it->halfD2Chi2DX2() ;
          WX   -= it->weight() * it->halfDChi2DX() ;
          chi2 -= w * trk.chi2() ;
          ++nremoved ;
        }
      }
    }
    if ( nremoved > 0 )
    {
      const bool success = W.InvertChol() ;
      if ( success )
      {
        const auto delta = W * WX ;
        const auto chi2  = m_sumchi2 - ROOT::Math::Dot(delta,WX) ;
        target.setCovMatrix( W ) ;
        target.setPosition( Gaudi::XYZPoint(delta(0),delta(1),delta(2) + m_refZ) ) ;
        target.setChi2AndDoF( chi2,nDoF() - 2* nremoved) ;
      } else {
        nremoved = -1 ;
      }
    }
    return nremoved ;
  }

  int PrimaryVertex::unbiasedPosition( const std::vector<const LHCb::Track*>& tracksToRemove,
                                       Gaudi::XYZPoint& pos, Gaudi::SymMatrix3x3& cov) const
  {
    const auto N = tracksToRemove.size() ;
    std::vector< VeloSegmentID > idsToRemove(N) ;
    for ( unsigned int i = 0; i<N; ++i )
      idsToRemove[i] = uniqueVeloSegmentID(*tracksToRemove[i]) ;
    return unbiasedPosition(idsToRemove,pos,cov) ;
  }


  int PrimaryVertex::unbiasedPosition( const std::vector<VeloSegmentID>& tracksToRemove,
                                       Gaudi::XYZPoint& pos, Gaudi::SymMatrix3x3& cov) const
  {
    int nremoved(0) ;
    // subtract all the tracks
    auto W  = m_sumhalfD2Chi2DX2 ;
    auto WX = m_sumhalfDChi2DX ;
    for( const auto& track : tracksToRemove )
    {
      const auto it = find(track) ;
      if ( it != m_tracks.end() )
      {
        const auto& trk = *it ;
        const auto w =  trk.weight() ;
        if ( w > 0 )
        {
          W  -= it->weight() * it->halfD2Chi2DX2() ;
          WX -= it->weight() * it->halfDChi2DX() ;
          ++nremoved ;
        }
      }
    }
    if ( nremoved > 0 )
    {
      const bool success = W.InvertChol() ;
      if ( success )
      {
        const auto delta = W * WX ;
        pos = Gaudi::XYZPoint( delta(0),delta(1),delta(2) + m_refZ) ;
        cov = W ;
      } else {
        pos = position() ;
        cov = covMatrix() ;
        nremoved = -1 ;
      }
    } else {
      pos = position() ;
      cov = covMatrix() ;
    }
    return nremoved ;
  }

  void PrimaryVertex::updateTukeyWeights(double maxchi2)
  {
    const Vector3 pos( position().x(),
                       position().y(),
                       position().z() - m_refZ );
    for( auto& track : m_tracks )
    {
      const auto chi2   = track.chi2(pos) ;
      const auto weight = chi2 < maxchi2 ? std::pow( 1.0 - (chi2/maxchi2), 2 ) : 0 ;
      track.setWeight( weight ) ;
    }
    initCache() ;
  }
}
