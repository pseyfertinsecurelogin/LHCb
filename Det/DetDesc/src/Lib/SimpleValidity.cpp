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
// GaudiKernel
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Time.h"

#include "DetDesc/SimpleValidity.h"

// ============================================================================
/** @file SimpleValidity.cpp
 *
 * Implementation file for class : SimpleValidity
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @date 17/11/2001
 */
// ============================================================================

// ============================================================================
/** standard (default) constructor
 */
// ============================================================================
SimpleValidity::SimpleValidity() : m_since( Gaudi::Time::epoch() ), m_till( Gaudi::Time::max() ) {}

// ============================================================================
/** standard (default) constructor
 *  @param since "since" time for validity range
 *  @param till  "till"  time for validity range
 */
// ============================================================================
SimpleValidity::SimpleValidity( const Gaudi::Time& since, const Gaudi::Time& till )
    : m_since( since ), m_till( till ) {}

// ============================================================================
/** (explicit) constructor from other
 *   IValidity object
 *   @param copy another IValidity object
 */
// ============================================================================
SimpleValidity::SimpleValidity( const IValidity& copy ) : m_since( copy.validSince() ), m_till( copy.validTill() ) {}

// ============================================================================
/** assignement from any IValidity object
 *  @param copy another IValidity object
 */
// ============================================================================
SimpleValidity& SimpleValidity::operator=( const IValidity& copy ) {
  m_since = copy.validSince();
  m_till  = copy.validTill();
  return *this;
}

// ============================================================================
/** is the Object valid? (it can be always invalid)
 *  @return true if object valid
 */
// ============================================================================
bool SimpleValidity::isValid() const { return m_since <= m_till; }

// ============================================================================
/** is the Object valid for a given time?
 *  @param  time  time to be checked
 *  @return true if objetc is valid for given time
 */
// ============================================================================
bool SimpleValidity::isValid( const Gaudi::Time& time ) const { return ( m_since <= time ) && ( time <= m_till ); }

// ============================================================================
/// set the validity range of the Object
// ============================================================================
void SimpleValidity::setValidity( const Gaudi::Time& since, const Gaudi::Time& till ) {
  m_since = since;
  m_till  = till;
}

// ============================================================================
/// set the validity time of the Object
// ============================================================================
void SimpleValidity::setValiditySince( const Gaudi::Time& since ) { m_since = since; }

// ============================================================================
/// set the validity time of the Object
// ============================================================================
void SimpleValidity::setValidityTill( const Gaudi::Time& till ) { m_till = till; }

// ============================================================================
// The End
// ============================================================================
