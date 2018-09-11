
// local
#include "Event/GhostTrackInfo.h"

// fillstream method
std::ostream& LHCb::GhostTrackInfo::fillStream(std::ostream& s) const
{
  s << "{ " << "classed as :";
  s << m_classification;
  s << std::endl << " }";
  return s;
}

LHCb::GhostTrackInfo::LinkPair LHCb::GhostTrackInfo::bestLink() const
{
  double tot = 0.0;
  std::pair<LHCb::MCParticle*,unsigned int> best(0,0);
  for ( const auto & l : m_linkMap )
  {
    if ( l.second > best.second ) { best = l; }
    tot += l.second;
  }
  const double purity = ( tot > 0 ? double(best.second) / tot : 0.0 );
  return { best.first,purity };
}
