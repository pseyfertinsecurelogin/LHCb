#include "Event/PrZip.h"
#include "GaudiKernel/GaudiException.h"
#include "LHCbMath/SIMDWrapper.h"
#include "SOAExtensions/ZipUtils.h"
#include <algorithm>
#include <execution>
#include <numeric>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utest_stl

#include <boost/test/unit_test.hpp>

class loopable {
public:
  template <typename dType, bool unwrap>
  typename std::conditional_t<unwrap, bool, typename dType::mask_v> return_true( const std::size_t i ) const {
    if constexpr ( unwrap ) {
      if ( i >= size() ) { throw GaudiException{"out of range access", "stl.cpp", StatusCode::FAILURE}; }
      return true;
    } else {
      if ( i + dType::size - 1 >= capacity() ) {
        throw GaudiException{"out of range access:\noffset = " + std::to_string( i ) + "\ncapacity = " +
                                 std::to_string( capacity() ) + "\nsimd size = " + std::to_string( dType::size ),
                             "stl.cpp", StatusCode::FAILURE};
      }
      return dType::mask_true();
    }
  }

  void push_back( int i ) {
    reserve( size() + 1 );
    m_storage.push_back( i );
  }
  std::size_t size() const { return m_storage.size(); }
  auto        capacity() const { return m_storage.capacity(); }
  void        reserve( const std::size_t i ) { m_storage.reserve( align_size( i ) ); }

  [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_f; }

private:
  Zipping::ZipFamilyNumber m_f{Zipping::generateZipIdentifier()};
  std::vector<int>         m_storage;
};

namespace TestProxies {
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( Proxy, dType, unwrap, loopable, m_actual_struct );
    auto return_true() const { return this->m_actual_struct->template return_true<dType, unwrap>( this->offset() ); }
    auto capacity() const { return this->m_actual_struct->capacity(); }
  };
} // namespace TestProxies

REGISTER_PROXY( loopable, TestProxies::Proxy );

BOOST_AUTO_TEST_CASE( test_stl_algorithms ) {

  loopable l;
  for ( int i = 0; i < 9; ++i ) { l.push_back( i ); }
  auto zip = LHCb::Pr::make_zip<SIMDWrapper::InstructionSet::SSE>( l );
  bool true_check;
  BOOST_CHECK_NO_THROW( true_check = std::all_of( zip.begin(), zip.end(), []( const auto chunk ) {
                          return all( ( !chunk.loop_mask() ) || chunk.return_true() );
                        } ) );
  BOOST_CHECK( true_check );

  std::size_t true_count;
  BOOST_CHECK_NO_THROW( true_count = std::transform_reduce(
                            std::execution::seq, zip.begin(), zip.end(), (std::size_t)0, std::plus<std::size_t>(),
                            []( const auto proxy ) { return popcount( proxy.loop_mask() && proxy.return_true() ); } ) );

  BOOST_CHECK_EQUAL( true_count, l.size() );

  decltype( zip.begin() ) firstfalse;
  BOOST_CHECK_NO_THROW( firstfalse = std::find_if_not(
                            zip.begin(), zip.end(), []( const auto proxy ) { return any( proxy.return_true() ); } ) );

  // BOOST_CHECK_EQUAL( firstfalse, zip.end() );
  BOOST_CHECK( firstfalse == zip.end() );
}
