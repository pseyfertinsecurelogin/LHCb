// $Id: $
#ifndef TESFINGERPRINT_H
#define TESFINGERPRINT_H 1

#include <map>
#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataSelector.h"
#include "GaudiKernel/StatusCode.h"

class IDataStoreLeaves;
class ISvcLocator;

/** @class TESFingerPrint TESFingerPrint.h
 *
 *
 *  @author Illya Shapoval
 *  @date   2011-05-11
 */
class TESFingerPrint final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  TESFingerPrint( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TESFingerPrint( ) = default; ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  /// Pointer to the (public) tool used to retrieve the objects in a file.
  IDataStoreLeaves *m_leavesTool = nullptr;

  /// Collection of objects being selected
  IDataSelector m_objects;

  /// Level of the heuristic analysis
  std::string m_heur_level;

  /// Counter map of nodes occurrences
  std::map<std::string, long> m_stat_map;
  /// Counter map of containers and their contents occurrences
  std::map<std::string, std::map<int,long> > m_cont_stat_map;
  /// File name for the TES finger print output
  std::string m_output_file_name;

};

#endif // TESFINGERPRINT_H
