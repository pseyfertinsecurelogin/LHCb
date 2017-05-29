
//-----------------------------------------------------------------------------
/** @file RichPIDsFromProtoParticlesAlg.h
 *
 * Header file for algorithm RichPIDsFromProtoParticlesAlg
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 29/03/2006
 */
//-----------------------------------------------------------------------------

#ifndef GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H
#define GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H 1

#include <string>

// Event
#include "Event/ProtoParticle.h"
#include "Event/RichPID.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
/** @class RichPIDsFromProtoParticlesAlg RichPIDsFromProtoParticlesAlg.h
 *
 *  Simple algorithm to recreate RichPID data objects from the data
 *  stored in the ProtoParticles
 *m_richPIDloc
 *  @author Chris Jones
 *  @date   2006-08-01
 */
//-----------------------------------------------------------------------------

class RichPIDsFromProtoParticlesAlg : public GaudiAlgorithm
{

public:

  /// Standard constructor
  RichPIDsFromProtoParticlesAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RichPIDsFromProtoParticlesAlg( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// Input location of ProtoParticles in TES
  std::string m_protoPloc;

  /// Output location of RichPIDs in TES
  std::string m_richPIDloc;

};

#endif // GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H
