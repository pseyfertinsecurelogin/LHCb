#ifndef CALOFUTUREMERGETAE_H
#define CALOFUTUREMERGETAE_H 1

// Include files
#include <iomanip>
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloDigit.h"
#include "Event/CaloAdc.h"

#include "futuredetails.h"

/** @class CaloFutureMergeTAE CaloFutureMergeTAE.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-10-24
 */
class CaloFutureMergeTAE : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureMergeTAE( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  void mergeDigits();
  void mergeAdcs();
  Gaudi::Property<std::vector<std::string>> m_slots = { this, "MergeSlots", { "T0", "Prev1", "Next1" } };
  Gaudi::Property<std::string> m_detectorName{ this, "Detector" };
  Gaudi::Property<double> m_threshold { this, "SumThreshold",  -256 };
  Gaudi::Property<double> m_slotcut{ this, "SlotThreshold"  , -256 };
  std::string m_locDigit;
  std::string m_outDigit;
  std::string m_locAdc;
  std::string m_outAdc;
  Gaudi::Property<futuredetails::source_t> m_data { this ,"OutputType", futuredetails::source_t::from_digit };

  Gaudi::Property<std::string> m_outputDataLoc { this, "OutputDataLocation", "" };
  Gaudi::Property<std::string> m_inExt { this, "inputExtension", "" };

  DeCalorimeter* m_calo = nullptr;

};

#endif // CALOFUTUREMERGETAE_H
