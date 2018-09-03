
#include "RichFutureUtils/RichSIMDMirrorData.h"

using namespace Rich::SIMD;

//=============================================================================

// Update the cached mirror information
void
MirrorData::update( const Mirrors &mirrors ) noexcept
{
  // loop over the scalar entries and update as needed
  for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
  {
    // the mirror pointer
    const auto m = mirrors[i];
    // If changed from before, update
    if ( UNLIKELY( m != cache_mirrors[i] ) )
    {
      // update cache mirror pointer
      cache_mirrors[i] = m;

      // Get the mirror data
      const auto mData = m->mirrorData();

      // Update the RoC
      m_RoCs[i] = mData.radius;

      // Update the plane params
      m_NormPs.normal._X[i] = mData.planeA;
      m_NormPs.normal._Y[i] = mData.planeB;
      m_NormPs.normal._Z[i] = mData.planeC;
      m_NormPs.D[i]         = mData.planeD;

      // update the CoC params
      m_CoCs._X[i] = mData.cocX;
      m_CoCs._Y[i] = mData.cocY;
      m_CoCs._Z[i] = mData.cocZ;
    }
  }
}
