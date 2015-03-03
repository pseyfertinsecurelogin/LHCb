// $Id: PackedVertex.cpp,v 1.3 2010-05-19 09:04:09 jonrob Exp $

// STL
#include <algorithm>

// local
#include "Event/PackedVertex.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

void VertexPacker::pack( const Data & vert,
                         PackedData & pvert,
                         PackedDataVector & pverts ) const
{
  if ( 1 == pverts.packingVersion() )
  {
    // technique
    pvert.technique = static_cast<int>( vert.technique() );
    pvert.chi2      = m_pack.fltPacked( vert.chi2() );
    pvert.nDoF      = vert.nDoF();
    pvert.x         = m_pack.position( vert.position().x() );
    pvert.y         = m_pack.position( vert.position().y() );
    pvert.z         = m_pack.position( vert.position().z() );

    // convariance Matrix
    const double err0 = safe_sqrt( vert.covMatrix()(0,0) );
    const double err1 = safe_sqrt( vert.covMatrix()(1,1) );
    const double err2 = safe_sqrt( vert.covMatrix()(2,2) );
    pvert.cov00 = m_pack.position( err0 );
    pvert.cov11 = m_pack.position( err1 );
    pvert.cov22 = m_pack.position( err2 );
    pvert.cov10 = m_pack.fraction( vert.covMatrix()(1,0), err1*err0 );
    pvert.cov20 = m_pack.fraction( vert.covMatrix()(2,0), err2*err0 );
    pvert.cov21 = m_pack.fraction( vert.covMatrix()(2,1), err2*err1 );

    // outgoing particles
    pvert.firstOutgoingPart = pverts.outgoingParticles().size();
    for ( SmartRefVector<LHCb::Particle>::const_iterator iP = vert.outgoingParticles().begin();
          iP != vert.outgoingParticles().end(); ++iP )
    {
      const LHCb::Particle * P = *iP;
      if ( P )
      {
        pverts.outgoingParticles().push_back( m_pack.reference64( &pverts,
                                                                  P->parent(),
                                                                  P->key() ) );
      }
    }
    pvert.lastOutgoingPart = pverts.outgoingParticles().size();

    //== Handles the ExtraInfo
    pvert.firstInfo = pverts.extras().size();
    for ( GaudiUtils::VectorMap<int,double>::iterator itE = vert.extraInfo().begin();
          vert.extraInfo().end() != itE; ++itE )
    {
      pverts.addExtra( (*itE).first, m_pack.fltPacked( (*itE).second ) );
    }
    pvert.lastInfo = pverts.extras().size();
  }
  else
  {
    std::ostringstream mess;
    mess << "Unknown packed data version " << (int)pverts.packingVersion();
    throw GaudiException( mess.str(), "VertexPacker", StatusCode::FAILURE );
  }
}

void VertexPacker::pack( const DataVector & verts,
                         PackedDataVector & pverts ) const
{
  pverts.data().reserve( verts.size() );

  for ( DataVector::const_iterator iD = verts.begin();
        iD != verts.end(); ++iD )
  {
    const Data & vert = **iD;

    // new packed data object
    pverts.data().push_back( PackedData() );
    PackedData & pvert = pverts.data().back();

    // Key
    pvert.key = vert.key();

    // fill physics info from vert to pvert
    pack( vert, pvert, pverts );
  }

}

void VertexPacker::unpack( const PackedData       & pvert,
                           Data                   & vert,
                           const PackedDataVector & pverts,
                           DataVector             & verts ) const
{
  if ( 1 == pverts.packingVersion() )
  {
    // technique
    vert.setTechnique( static_cast<Vertex::CreationMethod>(pvert.technique) );
    vert.setChi2AndDoF( m_pack.fltPacked( pvert.chi2), pvert.nDoF );
    vert.setPosition( Gaudi::XYZPoint( m_pack.position( pvert.x ),
                                       m_pack.position( pvert.y ),
                                       m_pack.position( pvert.z ) ) );

    // convariance Matrix
    const double err0 = m_pack.position( pvert.cov00 );
    const double err1 = m_pack.position( pvert.cov11 );
    const double err2 = m_pack.position( pvert.cov22 );
    Gaudi::SymMatrix3x3 & cov = *(const_cast<Gaudi::SymMatrix3x3*>(&vert.covMatrix()));
    cov(0,0) = err0 * err0;
    cov(1,0) = err1 * err0 * m_pack.fraction( pvert.cov10 );
    cov(1,1) = err1 * err1;
    cov(2,0) = err2 * err0 * m_pack.fraction( pvert.cov20 );
    cov(2,1) = err2 * err1 * m_pack.fraction( pvert.cov21 );
    cov(2,2) = err2 * err2;

    // outgoing particles
    for ( unsigned int iiP = pvert.firstOutgoingPart; iiP < pvert.lastOutgoingPart; ++iiP )
    {
      const long long & iP = pverts.outgoingParticles()[iiP];
      int hintID(0), key(0);
      m_pack.hintAndKey64( iP, &pverts, &verts, hintID, key );
      SmartRef<LHCb::Particle> ref(&verts,hintID,key);
      vert.addToOutgoingParticles( ref );
    }
    //== Handles the ExtraInfo
    for ( unsigned short int kEx = pvert.firstInfo; pvert.lastInfo > kEx; ++kEx )
    {
      const PackedDataVector::ExtraInfo& info = *(pverts.extras().begin()+kEx);
      vert.addInfo( info.first, m_pack.fltPacked( info.second ) );
    }
  }
  else
  {
    std::ostringstream mess;
    mess << "Unknown packed data version " << (int)pverts.packingVersion();
    throw GaudiException( mess.str(), "VertexPacker", StatusCode::FAILURE );
  }
}

void VertexPacker::unpack( const PackedDataVector & pverts,
                           DataVector             & verts ) const
{
  verts.reserve( pverts.data().size() );

  for ( PackedDataVector::Vector::const_iterator iD = pverts.data().begin();
        iD != pverts.data().end(); ++iD )
  {
    const PackedData & pvert = *iD;

    // make and save new pid in container
    Data * vert = new Data();
    verts.insert( vert, pvert.key );

    // Fill data from packed object
    unpack( pvert, *vert, pverts, verts );
  }

}

StatusCode VertexPacker::check( const DataVector & dataA,
                                const DataVector & dataB ) const
{
  StatusCode sc = StatusCode::SUCCESS;

  // Loop over data containers together and compare
  DataVector::const_iterator iA(dataA.begin()), iB(dataB.begin());
  for ( ; iA != dataA.end() && iB != dataB.end(); ++iA, ++iB )
  {
    sc = sc && check( **iA, **iB );
  }

  // Return final status
  return sc;
}

StatusCode VertexPacker::check( const Data & dataA,
                                const Data & dataB ) const
{
  // assume OK from the start
  bool ok = true;

  // checker
  const DataPacking::DataChecks ch(parent());

  // checks here

  // key
  ok &= dataA.key() == dataB.key();
  // technique
  ok &= dataA.technique() == dataB.technique();
  // Chi^2
  const double chiTol = std::max( dataA.chi2() * 1.0e-6, 1.0e-3 );
  ok &= ch.compareDoubles( "Chi^2", dataA.chi2(), dataB.chi2(), chiTol );
  // NDOF
  ok &= ch.compareInts( "nDOF", dataA.nDoF(), dataB.nDoF() );
  // Position
  ok &= ch.comparePoints( "Position", dataA.position(), dataB.position() );
  // Cov matrix
  const boost::array<double,3> tolDiag = {{ 5.0e-3, 5.0e-3, 5.0e-3 }};
  ok &= ch.compareCovMatrices<Gaudi::SymMatrix3x3,3>( "Covariance",
                                                      dataA.covMatrix(),
                                                      dataB.covMatrix(),
                                                      tolDiag, 2.0e-5 );

  // force printout for tests
  //ok = false;
  // If comparison not OK, print full information
  if ( !ok )
  {
    const std::string loc = ( dataA.parent() && dataA.parent()->registry() ? 
                              dataA.parent()->registry()->identifier() : "Not in TES" );
    parent().warning() << "Problem with Vertex data packing :-" << endmsg
                       << "  Original Vertex key=" << dataA.key() 
                       << " in '" << loc << "'" << endmsg
                       << dataA << endmsg
                       << "  Unpacked Vertex" << endmsg
                       << dataB << endmsg;
  }

  return ( ok ? StatusCode::SUCCESS : StatusCode::FAILURE );
}
