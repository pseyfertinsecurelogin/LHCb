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
#ifndef BOOLEINIT_H
#define BOOLEINIT_H 1

// Include files
#include "Event/ODIN.h"
#include "GaudiKernel/RndmGenerators.h"
#include "Kernel/LbAppInit.h"
#include <string>
#include <vector>

class IGenericTool;

/** @class BooleInit BooleInit.h
 *  First TopAlg for Boole. Initializes random number and fills memory histogram
 *
 *  @author Marco Cattaneo
 *  @date   2005-12-15
 */
class BooleInit : public LbAppInit {
public:
  /// Standard constructor
  BooleInit( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~BooleInit(); ///< Destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

protected:
  virtual void modifyOdin( LHCb::ODIN* odin ); ///< fills some Odin fields
  virtual void simpleOdin( LHCb::ODIN* odin ); ///< sets reasonable defaults for some Odin fields

private:
  IGenericTool* m_memoryTool; ///< Pointer to (private) memory histogram tool
  IGenericTool* m_odinTool;   ///< Pointer to odin encoding tool

  bool                m_modifyOdin;
  Rndm::Numbers       m_FlatDist;
  std::string         m_genCollisionLocation;
  std::vector<double> m_thresInteraction;
  std::vector<double> m_thresDiffractive;
  std::vector<double> m_thresElastic;
  double              m_threstrigger;
  bool                m_odinRndTrig;
};
#endif // BOOLEINIT_H
