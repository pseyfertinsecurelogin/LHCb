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

//--------------------------------------------------------------------------------
/** @file RichSmartIDHashFuncs.h
 *
 *  Header file to define hash functions for RichSmartIDs
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-11
 */
//--------------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHSMARTIDHASHFUNCS_H
#define RICHKERNEL_RICHSMARTIDHASHFUNCS_H 1

// GaudiKernel
#include "GaudiKernel/HashMap.h"

// Kernel
#include "Kernel/RichSmartID.h"

// std::hash specializations
#include <functional>
namespace std
{
  template <>
  struct hash< LHCb::RichSmartID >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct hash< const LHCb::RichSmartID >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct hash< LHCb::RichSmartID & >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct hash< const LHCb::RichSmartID & >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
} // namespace std

#ifdef __GNUC__

// hash_value function for Boost
namespace LHCb
{
  inline LHCb::RichSmartID::KeyType hash_value( const LHCb::RichSmartID &s )
  {
    boost::hash< LHCb::RichSmartID::KeyType > hasher;
    return hasher( s.key() );
  }
} // namespace LHCb

// GaudiUtils::Hash
namespace GaudiUtils
{
  template <>
  struct Hash< LHCb::RichSmartID >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct Hash< const LHCb::RichSmartID >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct Hash< LHCb::RichSmartID & >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
  template <>
  struct Hash< const LHCb::RichSmartID & >
  {
    inline constexpr LHCb::RichSmartID::KeyType operator()( const LHCb::RichSmartID &s ) const
      noexcept
    {
      return s.key();
    }
  };
} // namespace GaudiUtils

#endif

#endif // RICHKERNEL_RICHSMARTIDHASHFUNCS_H
