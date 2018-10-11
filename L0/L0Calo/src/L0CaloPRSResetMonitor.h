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
// $Id:  $
#ifndef L0CALOPRSRESETMONITOR_H
#define L0CALOPRSRESETMONITOR_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"

// from Event
#include "Event/L0DUBase.h"

// Forward declarations
class DeCalorimeter ;

/** @class L0CaloPRSResetMonitor L0CaloPRSResetMonitor.h
 *  Finds empty boards in L0Ele and send an alarm to reset the
 *  PRS in case of empty boards.
 *
 *  @author Patrick Robbe
 *  @date   10/08/2011
 */

class L0CaloPRSResetMonitor: public GaudiHistoAlg {

public:
  /// Standard constructor
  L0CaloPRSResetMonitor( const std::string& name , ISvcLocator* pSvcLocator ) ;

  /// Standard destructor
  virtual ~L0CaloPRSResetMonitor( ) ;

  /// Initialization
  StatusCode initialize()  override;

  /// Main execution routine: find empty boards
  StatusCode execute   ()  override;

private:

  DeCalorimeter * m_ecal ; ///< Pointer to Ecal detector element

  unsigned int m_nEvents ; ///< Counter of events

  std::string m_inputData ; ///<  Location of input data

  /** Frequency with which the search of empty boards is launched.
   *  Set by option.
   */
  unsigned int  m_updateFrequency ;

  /// Alarm flag set when one board is missing in the L0Electron trigger
  int  m_alarm ;

  /// map to store number of canditates per board
  std::map< int , int > m_electronCounter ;
};
#endif // L0CALOPRSRESETMONITOR_H
