
// local
#include "RichFutureUtils/RichSIMDGeomPhoton.h"

std::ostream &
Rich::SIMD::Future::RecoPhoton::fillStream( std::ostream &s ) const
{
  // Loop over SIMD entries
  s << "{ " << rich() << " ";
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  {
    if ( validityMask()[ i ] ) { s << scalarPhoton( i ); }
  }
  return s << " }";
}

Rich::Future::RecoPhoton::Vector
Rich::SIMD::Future::RecoPhoton::scalarPhotons() const
{
  // the vector to return
  Rich::Future::RecoPhoton::Vector photons;
  photons.reserve( SIMDFP::Size );
  // fill with valid photons
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  {
    if ( validityMask()[ i ] ) { photons.emplace_back( scalarPhoton( i ) ); }
  }
  // return
  return photons;
}
