#include "Kernel/PlatformInfo.h"

#include "VectorClass/instrset.h"

#include <algorithm>
#include <vector>

#ifndef BINARY_TAG
#error BINARY_TAG macro not defined
#endif

namespace
{
  // IMPORTANT: always add new platforms at the end of the list
  const std::vector<std::string> known_platforms = {
      "unknown",
      "i686-slc5-gcc43-dbg",
      "i686-slc5-gcc43-opt",
      "x86_64-centos7-gcc49-dbg",
      "x86_64-centos7-gcc49-do0",
      "x86_64-centos7-gcc49-opt",
      "x86_64-centos7-gcc62-dbg",
      "x86_64-centos7-gcc62-do0",
      "x86_64-centos7-gcc62-opt",
      "x86_64-centos7-gcc62-opt+o3",
      "x86_64-centos7-gcc7-opt",
      "x86_64-slc5-gcc43-dbg",
      "x86_64-slc5-gcc43-opt",
      "x86_64-slc5-gcc46-dbg",
      "x86_64-slc5-gcc46-opt",
      "x86_64-slc5-icc11-dbg",
      "x86_64-slc5-icc11-opt",
      "x86_64-slc6-clang32-opt",
      "x86_64-slc6-clang37-dbg",
      "x86_64-slc6-clang37-opt",
      "x86_64-slc6-clang38-opt",
      "x86_64-slc6-clang39-dbg",
      "x86_64-slc6-clang39-opt",
      "x86_64-slc6-gcc46-dbg",
      "x86_64-slc6-gcc46-opt",
      "x86_64-slc6-gcc47-opt",
      "x86_64-slc6-gcc48-dbg",
      "x86_64-slc6-gcc48-do0",
      "x86_64-slc6-gcc48-opt",
      "x86_64-slc6-gcc49-dbg",
      "x86_64-slc6-gcc49-do0",
      "x86_64-slc6-gcc49-opt",
      "x86_64-slc6-gcc61-opt",
      "x86_64-slc6-gcc62-dbg",
      "x86_64-slc6-gcc62-do0",
      "x86_64-slc6-gcc62-opt",
      "x86_64-slc6-icc13-opt",
      "x86_64+avx-centos7-gcc62-dbg+ofast",
      "x86_64+avx-centos7-gcc62-opt",
      "x86_64+avx-centos7-gcc62-opt+o3",
      "x86_64+avx-centos7-gcc62-opt+ofast",
      "x86_64+avx-centos7-gcc62-opt+ofast+g",
      "x86_64+avx+fma-centos7-gcc62-opt",
      "x86_64+avx2-centos7-gcc62-opt",
      "x86_64+avx2+fma-centos7-gcc62-opt",
      "x86_64+avx2+fma-centos7-gcc62-opt+o3",
      "x86_64-ubuntu1704-gcc63-opt",
  };

  const std::string invalid_platform = "'invalid platform id'";

  const std::vector<std::string> known_instruction_sets = {
      "80386", "SSE", "SSE2", "SSE3", "Supplementary SSE3 (SSSE3)", "SSE4.1", "SSE4.2", "AVX", "AVX2", "AVX512F",
  };

// special trick to add quotes around a macro value
// see https://stackoverflow.com/a/20632065
#define STR_( X ) #X
#define STR( X ) STR_( X )
  const auto binary_tag_name = STR( BINARY_TAG );
#undef STR
#undef STR_

  const std::uint16_t binary_tag_id =
      ( find( begin( known_platforms ), end( known_platforms ), binary_tag_name ) == end( known_platforms ) )
          ? 0
          : static_cast<std::uint16_t>( find( begin( known_platforms ), end( known_platforms ), binary_tag_name ) -
                                        begin( known_platforms ) );
}
namespace LHCb
{
  std::uint16_t PlatformInfo::s_hostInstrSetLevel = static_cast<std::uint16_t>( instrset_detect() );

  PlatformInfo::PlatformInfo() : m_binaryId( binary_tag_id ), m_hostInstrSetLevel( s_hostInstrSetLevel ) {}

  const std::string& PlatformInfo::binaryTag() const
  {
    return ( m_binaryId < known_platforms.size() ) ? known_platforms[m_binaryId] : invalid_platform;
  }

  const std::string& PlatformInfo::hostInstrSet() const
  {
    return ( m_hostInstrSetLevel < known_instruction_sets.size() ) ? known_instruction_sets[m_hostInstrSetLevel]
                                                                   : known_instruction_sets[0];
  }
}
