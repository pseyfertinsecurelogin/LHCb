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
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <utility>

#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// local
// ============================================================================
#include "Kernel/Counters.h"
// ============================================================================
/** @file
 *  Implementaton for simpel TES-counters
 *
 *  @see Gaudi::Counter
 *  @see Gaudi::Counters
 *  @see Gaudi::Numbers
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date   2010-11-09
 *
 */
// ============================================================================
namespace {
  // ==========================================================================
  /** @var CLID_Counter
   *  The uinque indentifier/CLID for class Gaudi::Counter
   *  @see Gaudi::Counter
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date   2010-11-09
   */
  const CLID CLID_Counter = 150;
  // ==========================================================================
  /** @var CLID_Counters
   *  The uinque indentifier/CLID for class Gaudi::Counters
   *  @see Gaudi::Counters
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date   2010-11-09
   */
  const CLID CLID_Counters = 151;
  // ==========================================================================
  /** @var CLID_Numbers
   *  The uinque indentifier/CLID for class Gaudi::Numbers
   *  @see Gaudi::Numbers
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date   2010-11-09
   */
  const CLID CLID_Numbers = 152;
  // ==========================================================================
} // namespace
// ============================================================================
// constructor from counter
// ============================================================================
Gaudi::Counter::Counter( StatEntity counter, std::string description )
    : DataObject(), m_counter( std::move( counter ) ), m_description( std::move( description ) ) {}
// ============================================================================
// retrieve the unique object identifier  (virtual)
// ============================================================================
const CLID& Gaudi::Counter::clID() const { return Gaudi::Counter::classID(); }
// ============================================================================
// retrieve the unique object identifier  (static)
// ============================================================================
const CLID& Gaudi::Counter::classID() { return CLID_Counter; }
// ============================================================================
// printout  to std::ostream
// ============================================================================
std::ostream& Gaudi::Counter::fillStream( std::ostream& o ) const { return o; }
// ============================================================================

// ============================================================================
// constructor from counters
// ============================================================================
Gaudi::Counters::Counters( Gaudi::Counters::Map counters ) : DataObject(), m_counters( std::move( counters ) ) {}
// ============================================================================
// retrieve the unique object identifier  (virtual)
// ============================================================================
const CLID& Gaudi::Counters::clID() const { return Gaudi::Counters::classID(); }
// ============================================================================
// retrieve the unique object identifier  (static)
// ============================================================================
const CLID& Gaudi::Counters::classID() { return CLID_Counters; }
// ============================================================================
// printout  to std::ostream
// ============================================================================
std::ostream& Gaudi::Counters::fillStream( std::ostream& o ) const { return o; }

// ============================================================================
// constructor from counters
// ============================================================================
Gaudi::Numbers::Numbers( Gaudi::Numbers::Map counters ) : DataObject(), m_counters( std::move( counters ) ) {}
// ============================================================================
// retrieve the unique object identifier  (virtual)
// ============================================================================
const CLID& Gaudi::Numbers::clID() const { return Gaudi::Numbers::classID(); }
// ============================================================================
// retrieve the unique object identifier  (static)
// ============================================================================
const CLID& Gaudi::Numbers::classID() { return CLID_Numbers; }
// ============================================================================
// printout  to std::ostream
// ============================================================================
std::ostream& Gaudi::Numbers::fillStream( std::ostream& o ) const { return o; }

// ============================================================================
// The END
// ============================================================================
