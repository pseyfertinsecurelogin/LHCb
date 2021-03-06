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
#ifndef L0CALOCANDIDATESFROMRAW_H
#define L0CALOCANDIDATESFROMRAW_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "L0Base/L0FromRawBase.h"

// from Event
#include "Event/L0DUBase.h"

#include "L0CaloCandidatesFromRawBank.h"

/** @class L0CaloCandidatesFromRaw L0CaloCandidatesFromRaw.h
 *
 *
 *  @author Olivier Callot
 *  @date   2003-12-15
 */
class L0CaloCandidatesFromRaw : public L0FromRawBase {
public:
  /// Standard constructor
  L0CaloCandidatesFromRaw( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~L0CaloCandidatesFromRaw(); ///< Destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

protected:
private:
  /** Converts type from L0DUBase::CaloType to L0DUBase::FiberType
   */
  L0DUBase::Fiber::Type fiberType( const int type ) const {
    switch ( type ) {
    case L0DUBase::CaloType::Electron:
      return L0DUBase::Fiber::CaloElectron;
      break;
    case L0DUBase::CaloType::Photon:
      return L0DUBase::Fiber::CaloPhoton;
      break;
    case L0DUBase::CaloType::Pi0Global:
      return L0DUBase::Fiber::CaloPi0Global;
      break;
    case L0DUBase::CaloType::Pi0Local:
      return L0DUBase::Fiber::CaloPi0Local;
      break;
    case L0DUBase::CaloType::Hadron:
      return L0DUBase::Fiber::CaloHadron;
      break;
    case L0DUBase::CaloType::SumEt:
      return L0DUBase::Fiber::CaloSumEt;
      break;
    case L0DUBase::CaloType::SpdMult:
      return L0DUBase::Fiber::CaloSpdMult;
      break;
    }
    return L0DUBase::Fiber::CaloPhoton;
  }

  std::string                  m_extension; ///< Adedd to default name, allows comparisons
  L0CaloCandidatesFromRawBank* m_convertTool;

  // Fiber plug
  Gaudi::Property<std::vector<int>> m_fibers{this, "PlugFibers"};
  bool                              plugFiber( L0DUBase::Fiber::Type type ) {
    if ( m_fibers.size() == 0 ) return true;
    for ( std::vector<int>::iterator ifiber = m_fibers.begin(); m_fibers.end() != ifiber; ifiber++ ) {
      if ( (L0DUBase::Fiber::Type)*ifiber == type ) return true;
    }
    return false;
  }
};
#endif // L0CALOCANDIDATESFROMRAW_H
