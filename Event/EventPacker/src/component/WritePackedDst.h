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
#ifndef WRITEPACKEDDST_H
#define WRITEPACKEDDST_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/RawEvent.h"
#include "PackedBank.h"

/** @class WritePackedDst WritePackedDst.h
 *  Write the packed DST as a MDF file
 *
 *  @author Olivier Callot
 *  @date   2008-12-01
 */
class WritePackedDst : public GaudiAlgorithm
{

public:

  /// Standard constructor
  WritePackedDst( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

  void storeInBlob( PackedBank& pBnk,
                    const void* data,
                    unsigned int nb,
                    unsigned int bSize );

private:

  std::vector<std::string> m_containers;
  LHCb::RawEvent* m_dst = nullptr;
  unsigned int m_blobNumber{0};
  unsigned int m_bankNb{0};

};

#endif // WRITEPACKEDDST_H
