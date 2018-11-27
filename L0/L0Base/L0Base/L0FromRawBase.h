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
#ifndef L0FROMRAWBASE_H
#define L0FROMRAWBASE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "DAQKernel/DecoderAlgBase.h"

/** @class L0FromRawBase L0FromRawBase.h
 *  Base class for the L0 banks decoding algorithms
 *
 *  @author Julien Cogan
 *  @date   2009-10-20
 */
class L0FromRawBase : public Decoder::AlgBase {
public:
  /// Standard constructor
  L0FromRawBase( const std::string& name, ISvcLocator* pSvcLocator );

protected:

  /**
     Scan the list of possible location and select the first existing one.
   */
  StatusCode selectRawEventLocation(std::string &location);
  std::vector<std::string> rawEventLocations(){return m_rawEventLocations;};

/** If algorithm should write the processor data on TES
 */
  bool writeProcData() const { return m_writeProcData ; } ;

/** If algorithm should write the L0XXCandidates or L0DUReport on TES
 */
  bool writeOnTES() const { return m_writeOnTES ; } ;

  /** The suffix to append to the default locations where
   *  to store the objects on the TES
   */
  const std::string& l0context() const { return m_l0context ; } ;

  /** returns the location where to write the data on TES
   *  @param[in] standardLocation "default" path where objects are stored
   */
  std::string dataLocation( const std::string & standardLocation ) const {
    return ( rootInTES() + standardLocation + l0context() ) ; } ;

  bool useRootInTES() const { return m_useRootInTES; }
  bool statusOnTES() const { return m_statusOnTES; }
private:
  Gaudi::Property<bool> m_writeProcData  { this, "WriteProcData", false };    ///< Flag to activate the writing of the processor data
  Gaudi::Property<bool> m_writeOnTES {this, "WriteOnTES", true };       ///< Flag to activate the writing of the Candidates/Report on the TES
  Gaudi::Property<std::string> m_l0context { this, "L0Context"} ; ///< Suffix appended to the default location in TES

  bool m_statusOnTES = true;      ///< Flag to activate the writing of the decoding status on the TES
  bool m_useRootInTES;     ///< Flag to use RootInTES in the raw bank location

};
#endif // L0FROMRAWBASE_H
