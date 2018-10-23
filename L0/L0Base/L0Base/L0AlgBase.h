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
#ifndef L0ALGBASE_H
#define L0ALGBASE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"


/** @class L0AlgBase L0AlgBase.h
 *
 *
 *  @author Julien Cogan
 *  @date   2009-10-20
 */
class L0AlgBase : public GaudiHistoAlg {
public:
  using GaudiHistoAlg::GaudiHistoAlg;

protected:

/** if algorithm should write L0XXXCandidates on TES
 *  The location will be the "default" location + l0context
 *  as suffix
 */
  bool writeOnTES() const { return m_writeOnTES ; } ;

  /** The suffix to append to the default locations where
   *  to store the objects on the TES
   */
  std::string l0context() const { return m_l0context ; } ;

  /** returns the location where to write the data on TES
   *  @param[in] standardLocation "default" path where objects are stored
   */
  std::string dataLocation( const std::string & standardLocation ) const {
    return ( rootInTES() + standardLocation + l0context() ) ; } ;

  /** If algorithm should write the raw banks it emulates
   */
  bool writeBanks() const { return m_writeBanks ; } ;

private:
  Gaudi::Property<bool> m_writeBanks { this,       "WriteBanks"  , true}; ///< Flag to activate the writing of the raw banks
  Gaudi::Property<bool> m_writeOnTES { this,       "WriteOnTES"  , false};///< Flag to activate the writing of the Candidates/Report on the TES
  Gaudi::Property<std::string> m_l0context { this, "L0Context"   , ""};   ///< Suffix appended to the default location in TES

};
#endif // L0ALGBASE_H
