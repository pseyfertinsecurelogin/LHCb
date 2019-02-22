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
#ifndef RECEVENTTIME_H
#define RECEVENTTIME_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface

/** Simple implementation of IEventTimeDecoder that takes the event
 *  time from LHCb::RecHeader.
 *
 *  @author Marco Clemencic
 *  @date   2010-09-23
 */

class RecEventTime final : public extends1<GaudiTool, IEventTimeDecoder> {

public:
  /// Standard constructor
  RecEventTime( const std::string& type, const std::string& name, const IInterface* parent );

  virtual ~RecEventTime() = default; ///< Destructor

  // --- implementation of IEventTimeDecoder ---
  /// Retrieve the event time from RecHeader (if present).
  /// @return The time of current event.
  Gaudi::Time getTime() const override;

private:
  /// Location of the RecHeader in the transient store.
  std::string m_recHeaderLoc;
};

#endif /* RECEVENTTIME_H */
