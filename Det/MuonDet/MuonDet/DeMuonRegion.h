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
// ============================================================================
#ifndef MUONDET_DEMUONREGION_H
#define MUONDET_DEMUONREGION_H 1
#include <memory>
#include "GaudiKernel/MsgStream.h"

// Include files
#include "DetDesc/DetectorElement.h"

/** @class DeMuonRegion DeMuonRegion.h MuonDet/DeMuonRegion.h
 *
 *  Detector element class for a region in the muon system
 *
 *  The Xml converted for this object also makes all of the chamber objects
 *  at the same time.
 *
 *  @author David Hutchcroft
 *  @date   21/01/2002
 */

/// Class ID of DeMuonRegion
static const CLID CLID_DEMuonRegion = 11005;

class DeMuonRegion: public DetectorElement {

public:
  /// Constructor, empty
  DeMuonRegion() = default;


  inline const CLID& clID() const override { return classID(); }

  static const CLID& classID(){  return CLID_DEMuonRegion;  }

private:

  /// Access to Msgstream object
  inline MsgStream & msgStream() const
  {
    if ( UNLIKELY(!m_msgStream) ) m_msgStream.reset( new MsgStream(msgSvc(),name()));
    return *m_msgStream;
  }

private:

  mutable std::unique_ptr<MsgStream> m_msgStream;

};

#endif    // MUONDET_DEMUONREGION_H
