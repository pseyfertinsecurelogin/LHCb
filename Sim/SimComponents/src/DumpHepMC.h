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
#ifndef SIM_DUMPHEPMC_H
#define SIM_DUMPHEPMC_H 1

#include "HepMC/GenEvent.h"

#include "GaudiAlg/GaudiAlgorithm.h"

class DumpHepMC : public GaudiAlgorithm
{
public:

  /// the actual type of container with addresses
  typedef std::vector<std::string> Addresses ;

public:

  /** execution of the algoritm
   *  @see IAlgorithm
   *  @return status code
   */
  StatusCode execute() override;

  /** standard constructor
   *  @see GaudiAlgorithm
   *  @see      Algorithm
   *  @see      AlgFactory
   *  @see     IAlgFactory
   *  @param name algorithm instance's name
   *  @param iscv pointer to Service Locator
   */
  DumpHepMC ( const std::string& name ,
              ISvcLocator*       isvc ) ;

private:

  Addresses  m_addresses ;

  /// Print HepMC::GenEvent ordering particles according to barcodes
  void orderedPrint( const HepMC::GenEvent * theEvent ,
                     std::ostream & ostr ) const ;

  /// Print HepMC::GenVertex ordering particles according to barcodes
  void orderedVertexPrint( HepMC::GenVertex * theVertex ,
                           std::ostream & ostr ) const ;
};

// ============================================================================
// The END
// ============================================================================
#endif // SIM_DUMPHEPMC_H
// ============================================================================
