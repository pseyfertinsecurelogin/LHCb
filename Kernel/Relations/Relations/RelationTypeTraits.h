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
#ifndef RELATIONS_RELATIONTYPETRAITS_H
#define RELATIONS_RELATIONTYPETRAITS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
#include <vector>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/Bases.h"
#include "Relations/Entry.h"
#include "Relations/ObjectTypeTraits.h"
#include "Relations/Range.h"
#include "Relations/TraitsHelpers.h"
// ============================================================================
namespace Relations {
  // ==========================================================================
  /** @struct RelationTypeTraits RelationTypeTraits.h
   *
   *  Type traits structure for definition of
   *  data types and operation
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   06/02/2002
   */
  template <class FROM, class TO>
  struct RelationTypeTraits {
    // ========================================================================
    /// the actual enum to distinguish the relation type
    // enum Type { weighted = false };
    // ========================================================================
    /// the actual type of the entry
    // typedef typename Relations::Entry_<FROM, TO> Entry;
    /// type traits for "FROM" object
    // typedef typename Entry::FromTypes FromTypes;
    /// type traits for "TO" object
    // typedef typename Entry::ToTypes ToTypes;
    ///
    // typedef typename Entry::From_ From_;
    // typedef typename Entry::From  From;
    ///
    // typedef typename Entry::To_ To_;
    // typedef typename Entry::To  To;
    /// "less" function object for "From" objects
    // typedef typename Entry::LessF LessF;
    /// "less" function object for "To" objects
    // typedef typename Entry::LessT LessT;
    /// "equality" function object for "To" objects
    // typedef typename Entry::EqualT EqualT;
    /// "less" function object for "From" objects
    // using LessByFrom = TraitsHelpers::LessByFrom<LessF>;
    // ========================================================================
    /** @struct  EqualByTo
     *  equality criteria using only "To" field
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   27/01/2002
     */
    // using EqualByTo = TraitsHelpers::EqualByTo<EqualT>;
    // ========================================================================
    /// definition of the internal storage
    // typedef std::vector<Entry> Entries;
    /// definition of the standard iterator types
    // typedef typename Entries::const_iterator iterator;
    /// the actual type fo Range
    // typedef Relations::Range_<Entries> Range;
    /** technical definitions, useful for  for implementation
     *  ordinary users should not use them
     */
    // typedef typename Entries::iterator       IT;
    // typedef typename Entries::const_iterator CIT;
  }; // end of struct
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RELATIONTYPETRAITS_H
