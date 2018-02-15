
#include "RichFutureUtils/RichSIMDMirrorData.h"

using namespace Rich::SIMD;

//=============================================================================

// Update the cached mirror information
void MirrorData::update( const Mirrors & mirrors ) noexcept
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
      // Update the RoC
      m_RoCs[i] = m->radius();
      // Update the plane params
      const auto & p = m->centreNormalPlane();
      m_NormPs.normal._X[i] = p.A();
      m_NormPs.normal._Y[i] = p.B();
      m_NormPs.normal._Z[i] = p.C();
      m_NormPs.D[i]         = p.D();
      // update the CoC params
      const auto & n = m->centreOfCurvature();
      m_CoCs._X[i] = n.X();
      m_CoCs._Y[i] = n.Y();
      m_CoCs._Z[i] = n.Z();
    }
  }
}
