/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

namespace Relations::TraitsHelpers {
  template <typename T>
  using Entry = typename T::Entry;
  template <typename T>
  using From_ = typename T::From_;
  template <typename T>
  using To_ = typename T::To_;
  template <typename T>
  using Weight_ = typename T::Weight_;
  template <typename T>
  using From = typename T::From;
  template <typename T>
  using To = typename T::To;
  template <typename T>
  using Weight = typename T::Weight;
  template <typename T>
  using Entries = typename T::Entries;
  template <typename T>
  using Range = typename T::Range;
  template <typename T>
  using Less = typename T::Less;
  template <typename T>
  using LessF = typename T::LessF;
  template <typename T>
  using LessT = typename T::LessT;
  template <typename T>
  using LessW = typename T::LessW;
  template <typename T>
  using EqualT = typename T::EqualT;
  template <typename T>
  using Equal = typename T::Equal;
  template <typename T>
  using Less1 = typename T::Less1;
  template <typename T>
  using Less2 = typename T::Less2;

  template <typename Cmp>
  struct LessByFrom {
    template <typename E>
    bool operator()( const E& lhs, const E& rhs ) const {
      return Cmp{}( lhs.m_from, rhs.m_from );
    }
  };
  template <typename Cmp>
  struct LessByWeight {
    template <typename E>
    bool operator()( const E& lhs, const E& rhs ) const {
      return Cmp{}( lhs.m_weight, rhs.m_weight );
    }
  };
  template <typename Cmp>
  struct EqualByTo {
    template <typename E>
    bool operator()( const E& lhs, const E& rhs ) const {
      return Cmp{}( lhs.m_to, rhs.m_to );
    }
  };
} // namespace Relations::TraitsHelpers
