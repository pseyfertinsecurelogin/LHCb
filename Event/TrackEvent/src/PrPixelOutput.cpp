#include "Event/PrPixelOutput.h"
#include "Event/TrackTags.h"

foo::PrPixelOutputForTracking::operator LHCb::Event::v2::Track() {
  LHCb::Event::v2::Track retval;

  retval.addToStates(this->state); // yes, this can be left out, but prefer for readability. readability is in the eye of the reader
  retval.setLhcbIDs( this->lhcbIDs, LHCb::Tag::Sorted_tag{} );
  retval.setFlags(this->m_flags);
  // skip chi2perdof, should get done by whoever copies for their purpuse
  // skip likelihood, ghostprob, extrainfos, history

  return retval;
}
