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
#define BOOST_TEST_MODULE test_RunChangeHandler
#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "../../src/RunChangeHandlerSvc.h"

#include <boost/io/ios_state.hpp>

#include <fstream>
#include <iostream>

struct Fixture {
  Fixture() {
    std::ofstream f("test_rch_data_1.txt");
    f << "just some data";
  }
  ~Fixture() {
    boost::filesystem::remove("test_rch_data_1.txt");
  }
};

using FileHasher = RunChangeHandlerSvc::FileHasher;
using Hash_t = RunChangeHandlerSvc::FileHasher::Hash_t;

namespace std {
  ostream& operator<< (ostream& os, const Hash_t& hash) {
    boost::io::ios_all_saver ias(os);
    os << hex;
    for(unsigned short c: hash) {
      os << c;
    }
    return os;
  }
}

BOOST_AUTO_TEST_SUITE( hashing )

BOOST_AUTO_TEST_CASE( no_file )
{
  const FileHasher h;
  BOOST_CHECK_THROW( h("no_such_file"), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( correctness, Fixture )
{
  const FileHasher h;
  Hash_t expected = {205, 246, 69, 24, 108, 213, 185, 65, 184, 103, 147, 43, 129, 66, 168, 187, 253, 51, 179, 228};

  BOOST_CHECK_EQUAL( h("test_rch_data_1.txt"), expected );
}

BOOST_FIXTURE_TEST_CASE( cache, Fixture )
{
  const FileHasher h;
  Hash_t expected = {205, 246, 69, 24, 108, 213, 185, 65, 184, 103, 147, 43, 129, 66, 168, 187, 253, 51, 179, 228};

  BOOST_CHECK_EQUAL( h("test_rch_data_1.txt"), expected );

  {
    std::ofstream f("test_rch_data_1.txt");
    f << "junk";
  }

  Hash_t cached = expected;
  Hash_t newvalue = {201, 17, 41, 114, 18, 217, 85, 214, 80, 80, 37, 85, 216, 77, 226, 201, 34, 182, 228, 249};

  BOOST_CHECK_EQUAL( h("test_rch_data_1.txt"), cached );
  BOOST_CHECK_EQUAL( FileHasher{}("test_rch_data_1.txt"), newvalue );
}

BOOST_AUTO_TEST_SUITE_END()



struct PTFixture {
  PTFixture() {
    boost::filesystem::create_directory("path_template_data");
    boost::filesystem::create_directory("path_template_data/run");
    {
      std::ofstream f("path_template_data/run/42.txt");
      f << "42\n";
    }
    {
      std::ofstream f("path_template_data/run/43.txt");
      f << "42\n";
    }
    {
      std::ofstream f("path_template_data/run/12345.txt");
      f << "12345\n";
    }

    boost::filesystem::create_directory("path_template_data/0");
    boost::filesystem::create_directory("path_template_data/12");
    {
      std::ofstream f("path_template_data/0/42.txt");
      f << "42\n";
    }
    {
      std::ofstream f("path_template_data/0/43.txt");
      f << "42\n";
    }
    {
      std::ofstream f("path_template_data/12/12345.txt");
      f << "12345\n";
    }
  }
  ~PTFixture() {
    boost::filesystem::remove_all("path_template_data");
  }
  std::string fn;
};

using PathTemplate = RunChangeHandlerSvc::PathTemplate;\

BOOST_AUTO_TEST_SUITE( path_template, * boost::unit_test::fixture<PTFixture>() )

BOOST_AUTO_TEST_CASE( format_check )
{
  //BOOST_CHECK_THROW( PathTemplate("some/%x/path"), std::invalid_argument );
  BOOST_CHECK_THROW( PathTemplate("some/%d/%d/path"), std::invalid_argument );
  BOOST_CHECK_NO_THROW( PathTemplate("some/%d/path") );
  BOOST_CHECK_NO_THROW( PathTemplate("some/%s/path") );
}

BOOST_AUTO_TEST_CASE( simple_format )
{
  PathTemplate t{"path_template_data/run/%d.txt"};

  BOOST_TEST( t.changed(42, FileHasher{}) );
  BOOST_TEST( !t.changed(42, FileHasher{}) );
  BOOST_TEST( !t.changed(43, FileHasher{}) );
  BOOST_TEST( t.changed(12345, FileHasher{}) );
  const FileHasher with_cache{};
  BOOST_TEST( !t.changed(12345, with_cache) );

  {
    std::ofstream f("path_template_data/run/12345.txt");
    f << "junk";
  }
  BOOST_TEST( !t.changed(12345, with_cache) );
  BOOST_TEST( t.changed(12345, FileHasher{}) );
}

BOOST_AUTO_TEST_CASE( split_run_format )
{
  PathTemplate t{"path_template_data/%s.txt"};

  BOOST_TEST( t.changed(42, FileHasher{}) );
  BOOST_TEST( !t.changed(42, FileHasher{}) );
  BOOST_TEST( !t.changed(43, FileHasher{}) );
  BOOST_TEST( t.changed(12345, FileHasher{}) );
  const FileHasher with_cache{};
  BOOST_TEST( !t.changed(12345, with_cache) );

  {
    std::ofstream f("path_template_data/12/12345.txt");
    f << "junk";
  }
  BOOST_TEST( !t.changed(12345, with_cache) );
  BOOST_TEST( t.changed(12345, FileHasher{}) );
}


BOOST_AUTO_TEST_SUITE_END()
