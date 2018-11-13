#include "Event/PrPixelOutput.h"
#include "Event/TrackTags.h"

foo::PrPixelOutputForTracking::operator LHCb::Event::v2::Track() const {
  LHCb::Event::v2::Track retval;

  // actually it almost seems like the parameterized kalman doesn't even use that.
  // retval.addToStates(this->state); // yes, this can be left out, but prefer for readability. readability is in the eye of the reader
  retval.setLhcbIDs( this->lhcbIDs, LHCb::Tag::Unordered_tag{});
  retval.setFlags(this->m_flags);
  // skip chi2perdof, should get done by whoever copies for their purpuse
  // skip likelihood, ghostprob, extrainfos, history

  return retval;
}
