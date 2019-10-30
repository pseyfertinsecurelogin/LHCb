/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef RELATIONS_RELATIONWEIGTEDTYPETRAITS_H
#define RELATIONS_RELATIONWEIGTEDTYPETRAITS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/Bases.h"
#include "Relations/ObjectTypeTraits.h"
#include "Relations/Range.h"
#include "Relations/TraitsHelpers.h"
#include "Relations/WEntry.h"
// ============================================================================
namespace Relations {

  // ==========================================================================
  /** @struct RelationWeightedTypeTraits RelationWeightedTypeTraits.h
   *
   *  Type traits for relations with the weight ("link attributes")
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   06/02/2002
   */
  template <class FROM, class TO, class WEIGHT>
  struct RelationWeightedTypeTraits {
    // ========================================================================
    /// the actual enum to distinguish the relation type
    // enum Type { weighted = true };
    // ========================================================================
    /// the actual type of the entry
    // using Entry = typename Relations::WEntry_<FROM, TO, WEIGHT>;
    /// actual type of "FROM" object
    // typedef typename Entry::From_ From_;
    // typedef typename Entry::From  From;
    /// actual type of "TO" object
    // typedef typename Entry::To_ To_;
    // typedef typename Entry::To  To;
    /// actual type of "WEIGHT" object
    // typedef typename Entry::Weight_ Weight_;
    // typedef typename Entry::Weight  Weight;
    /// "less" function object for "From" objects
    // typedef typename Entry::LessF LessF;
    /// "less" function object for "To" objects
    // typedef typename Entry::LessT LessT;
    /// "less" function object for "Weight" objects
    // using LessW = typename Entry::LessW;
    /// "equality" function object for "To" objects
    // using EqualT = typename Entry::EqualT;
    /// "less" function object for "Entry" objects
    // using Less = std::less<>;
    /// definition of the internal storage
    // using Entries = std::vector<Entry>;
    /// the actual type for range
    // using Range = Relations::Range_<Entries>;
  }; //< end of struct
} // end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RELATIONWEIGHTEDTYPETRAITS
