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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestTrackV2
#include <boost/test/unit_test.hpp>
#include <type_traits>

#include "Event/State.h"

#include "Event/Track.h"
#include "Event/Track_v2.h"
#include "Kernel/LHCbID.h"
#include <sstream>
#include <ostream>


// The test fixture: Create a container of tracks
// This can be used as a common starting point for all tests
using Track  = LHCb::Event::v2::Track;
struct ExampleTracks {
  std::vector<Track> m_tracks;                  // collection of tracks

  ExampleTracks() {
    BOOST_TEST_MESSAGE("setup example tracks");
    auto& tr1 = m_tracks.emplace_back();

    tr1.setFlags(1);
    tr1.setType(Track::Types::Velo);

    tr1.addToLhcbIDs(LHCb::LHCbID(1));
    tr1.addToLhcbIDs(LHCb::LHCbID(3));
    tr1.addToLhcbIDs(LHCb::LHCbID(5));

    auto& tr2 = m_tracks.emplace_back();
    tr2.setFlags(1);
    tr2.setType(Track::Types::TT);

    tr2.addToLhcbIDs(LHCb::LHCbID(2));
    tr2.addToLhcbIDs(LHCb::LHCbID(4));
    tr2.addToLhcbIDs(LHCb::LHCbID(6));

    auto& tr3=m_tracks.emplace_back();
    tr3.setFlags(8);
    tr3.setType(Track::Types::Long);

    tr3.addToLhcbIDs(LHCb::LHCbID(7));
    tr3.addToLhcbIDs(LHCb::LHCbID(11));
    tr3.addToLhcbIDs(LHCb::LHCbID(13));
    tr3.addToLhcbIDs(LHCb::LHCbID(17));

    // add a State to this track
    Gaudi::TrackVector v(1,-1,1,-1,1);

    // build an example covariance matrix
    // put the upper triangle intp vector
    double val[15] = {1.1,0.21,0.31,0.41,0.51,    // first row
		      1.2,0.22,0.32,0.42,
		      1.3,0.23,0.33,
		      1.4,0.24,
		      1.5};
    ROOT::Math::SVector<double,15> c(val,15);
    Gaudi::TrackSymMatrix cov(c,false);  // the false flag ensure we se the upper triangle
    //std::cout << cov << std::endl;

    //double* ar=cov.Array();
    //for(unsigned int i=0;i<15;++i){
    //  std::cout << ar[i] << " , ";
    //}
    //std::cout << std::endl;


    LHCb::State newYork(v,cov,200.,LHCb::State::Location::EndVelo);
    tr1.addToStates(newYork);

    LHCb::State Michigan(v,cov,220.,LHCb::State::Location::ClosestToBeam);
    tr1.addToStates(Michigan);

    m_tracks.push_back(tr1);

  }

  ~ExampleTracks() {
    BOOST_TEST_MESSAGE("tear down example tracks");
  }

};


BOOST_AUTO_TEST_CASE(test_track_flags){
  auto tr = Track{};

  tr.setType(Track::Types::Long);
  BOOST_CHECK(tr.type()==Track::Types::Long);
  BOOST_CHECK(toString(Track::Types::Long)=="Long");
  std::ostringstream stream_types;
  stream_types<<Track::Types::Long;
  BOOST_CHECK(stream_types.str()=="Long");
  BOOST_CHECK(Track::TypesToType("Long")==Track::Types::Long);


  tr.setHistory(Track::History::PrForward);
  BOOST_CHECK(tr.history()==Track::History::PrForward);
  BOOST_CHECK(toString(Track::History::PrForward)=="PrForward");
  std::ostringstream stream_history;
  stream_history<<Track::History::PrForward;
  BOOST_CHECK(stream_history.str()=="PrForward");
  BOOST_CHECK(Track::HistoryToType("PrForward")==Track::History::PrForward);

  tr.setFlag(Track::Flags::Backward, true);
  BOOST_CHECK(tr.flag()==Track::Flags::Backward);
  tr.setFlag(Track::Flags::Backward, false);
  BOOST_CHECK((tr.flag()==Track::Flags::Backward)==false);
  BOOST_CHECK(toString(Track::Flags::Backward)=="Backward");
  std::ostringstream stream_flags;
  stream_flags<<Track::Flags::Backward;
  BOOST_CHECK(stream_flags.str()=="Backward");
  BOOST_CHECK(Track::FlagsToType("Backward")==Track::Flags::Backward);

  tr.setFitStatus(Track::FitStatus::Fitted);
  BOOST_CHECK(tr.fitStatus()==Track::FitStatus::Fitted);
  BOOST_CHECK(toString(Track::FitStatus::Fitted)=="Fitted");
  std::ostringstream stream_fitstatus;
  stream_fitstatus<<Track::FitStatus::Fitted;
  BOOST_CHECK(stream_fitstatus.str()=="Fitted");
  BOOST_CHECK(Track::FitStatusToType("Fitted")==Track::FitStatus::Fitted);
}

void print_ids(LHCb::span<LHCb::LHCbID const> ids){
  for (auto const & id : ids){
    std::cout<<id<<" "<<id.lhcbID()<<std::endl;
  }
  std::cout<<"Sorted "<<std::boolalpha<<std::is_sorted(ids.begin(), ids.end())<<std::endl;
}

BOOST_AUTO_TEST_CASE(test_track_lhcbids){
  std::vector<Track> tracks;
  tracks.reserve(3);
  auto& tr1 = tracks.emplace_back();
  tr1.setFlags(1);
  tr1.setType(Track::Types::Velo);
  auto velo_id = LHCb::LHCbID();
  velo_id.setDetectorType(LHCb::LHCbID::channelIDtype::VP);
  velo_id.setID(1);
  auto ut_id = LHCb::LHCbID();
  ut_id.setDetectorType(LHCb::LHCbID::channelIDtype::UT);
  ut_id.setID(3);
  auto ft_id = LHCb::LHCbID();
  ft_id.setDetectorType(LHCb::LHCbID::channelIDtype::FT);
  ft_id.setID(5);
  tr1.addToLhcbIDs(velo_id);
  tr1.addToLhcbIDs(ut_id);
  tr1.addToLhcbIDs(ft_id);

  BOOST_CHECK(tr1.isOnTrack(ut_id));
  BOOST_CHECK(tr1.isOnTrack(ft_id));
  BOOST_CHECK(tr1.isOnTrack(velo_id));

  auto& tr2 = tracks.emplace_back();
  velo_id.setID(10);
  tr2.addToLhcbIDs(velo_id);
  ut_id.setID(4);
  tr2.addToLhcbIDs(ut_id);
  ft_id.setID(6);
  tr2.addToLhcbIDs(ft_id);

  auto& tr3 = tracks.emplace_back();
  tr3.addToLhcbIDs(velo_id);
  tr3.addToLhcbIDs(ut_id);
  ft_id.setID(100);
  tr3.addToLhcbIDs(ft_id);
  tr3.addToLhcbIDs(ft_id);
  BOOST_CHECK(tr3.nLHCbIDs()==3);

  BOOST_CHECK(tracks[0].nCommonLhcbIDs(tracks[1])==0);
  BOOST_CHECK(tracks[1].nCommonLhcbIDs(tracks[2])==2);

  std::vector<LHCb::LHCbID> lhcbids{velo_id, ut_id, ft_id};
  BOOST_CHECK(std::is_sorted(begin(lhcbids),end(lhcbids))==false);
  auto tr = Track{};
  tr.setLhcbIDs(lhcbids, LHCb::Tag::Unordered);
  BOOST_CHECK(std::is_sorted(std::begin(tr.lhcbIDs()),std::end(tr.lhcbIDs())));

  auto ids = tr.lhcbIDs();
  BOOST_CHECK(std::is_sorted(std::begin(ids),std::end(ids)));

  tr.addToLhcbIDs(velo_id);
  BOOST_CHECK(tr.lhcbIDs().size()==3);

  ut_id.setID(100);
  ft_id.setID(20);
  std::vector<LHCb::LHCbID> more_lhcbids{velo_id, ut_id, ft_id};
  std::sort(begin(more_lhcbids), end(more_lhcbids));
  //tr.addSortedToLhcbIDs(more_lhcbids);
  tr.addToLhcbIDs(more_lhcbids, LHCb::Tag::Sorted);
  BOOST_CHECK(tr.lhcbIDs().size()==5);
  ids = tr.lhcbIDs();
  BOOST_CHECK(std::is_sorted(std::begin(ids),std::end(ids)));

  ut_id.setID(101);
  ft_id.setID(21);
  std::vector<LHCb::LHCbID> even_more_lhcbids{velo_id, ut_id, ft_id};
  BOOST_CHECK(std::is_sorted(begin(even_more_lhcbids),end(even_more_lhcbids))==false);
  BOOST_CHECK(tr.addToLhcbIDs(even_more_lhcbids, LHCb::Tag::Unordered)==false);
  ids = tr.lhcbIDs();
  BOOST_CHECK(std::is_sorted(std::begin(ids),std::end(ids)));
  BOOST_CHECK(tr.lhcbIDs().size()==7);

}

BOOST_AUTO_TEST_CASE(test_track_constructors){
  static_assert(std::is_default_constructible_v<Track>);
  static_assert(std::is_move_constructible_v<Track>);
  static_assert(std::is_nothrow_move_constructible_v<Track>); // FIXME: waiting for gaudi/Gaudi!791
  static_assert(std::is_copy_constructible_v<Track>);
  // static_assert(std::is_nothrow_copy_constructible_v<Track>); // as long as we use heap memory, this is not going to be possible...
}
BOOST_AUTO_TEST_CASE(test_track_assignments){
  static_assert(std::is_move_assignable_v<Track>);
  static_assert(std::is_nothrow_move_assignable_v<Track>);
  static_assert(std::is_copy_assignable_v<Track>);
  // static_assert(std::is_nothrow_copy_assignable_v<Track>); // as long as we use heap memory, this is not going to be possible
  static_assert(std::is_assignable_v<Track&,Track>);
  static_assert(std::is_nothrow_assignable_v<Track&,Track>);
}

BOOST_AUTO_TEST_CASE(test_track_object_size){
  auto tr = Track{};
  BOOST_CHECK( sizeof(tr)==152 );
}


