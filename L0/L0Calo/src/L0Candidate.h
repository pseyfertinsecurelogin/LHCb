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
#ifndef L0CALO_L0CANDIDATE_H
#define L0CALO_L0CANDIDATE_H 1

// From Event
#include "Event/L0CaloCandidate.h"
#include "Event/L0DUBase.h"
#include "Event/L0ProcessorData.h"

// From Kernel
#include "Kernel/CaloCellID.h"

class DeCalorimeter;

/** @class L0Candidate
 *
 *  @author  Olivier Callot
 *  @date    4 October 2000
 *  This is a local class for temporary candidates, ID and Et, with saturation.
 *  This is internal to the algorithm, and is a termporary storage of the
 *  candidates, during the process of selecting the highest.
 */
class L0Candidate {
public:
  /** Constructor
   * @param det : The detector element pointer
   */
  L0Candidate( DeCalorimeter* det ) : m_det( det ), m_et( 0 ), m_ID(){};

  /** Constructor
   * @param cand : candidate from L0CaloCandidate type
   */
  L0Candidate( LHCb::L0CaloCandidate* cand ) : m_det( 0 ), m_et( cand->etCode() ), m_ID( cand->id() ){};

  ///	Destructor
  ~L0Candidate(){};

  /** update the information on a candidate
   *  @param et Integer transverse energy
   *  @param ID LHCb::CaloCellID location of the candidate
   */
  void setCandidate( int et, LHCb::CaloCellID ID );

  /** Save the candidate in the output container as L0ProcessorData
   *  @param type : Type of candidate, from L0DUFiber
   *  @param L0Calo : Container of candidates, to which the current object
   *                  is added after being properly formatted.
   */
  void saveCandidate( L0DUBase::Fiber::Type type, LHCb::L0ProcessorDatas* L0Calo ) {
    unsigned int word = 0;
    if ( L0DUBase::Fiber::CaloSumEt == type )
      word = 0x10000 + ( m_et << L0DUBase::Calo::Sum::Shift );
    else if ( L0DUBase::Fiber::CaloSpdMult == type )
      word = 0x10000 + ( m_et << L0DUBase::Calo::Sum::Shift );
    else
      word = 0x10000 | ( m_ID.index() << L0DUBase::Calo::Address::Shift ) | ( m_et << L0DUBase::Calo::Et::Shift );
    LHCb::L0ProcessorData* temp = new LHCb::L0ProcessorData( type, word );
    L0Calo->add( temp );
  }

  /// Return the transverse energy of the candidate
  int et() const { return m_et; };
  /// Return the CellID of the 2x2 cluster
  LHCb::CaloCellID ID() const { return m_ID; };
  /// Return the detector element associated with the candidate
  DeCalorimeter* det() const { return m_det; };

private:
  DeCalorimeter*   m_det; ///< Pointer to detector element
  int              m_et;  ///< Transverse energy (saturated)
  LHCb::CaloCellID m_ID;  ///< CellId of the 2x2 cluster
};
#endif // L0CALO_L0CANDIDATE_H
