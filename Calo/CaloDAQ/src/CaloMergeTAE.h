#ifndef CALOMERGETAE_H
#define CALOMERGETAE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloDigit.h"
#include "Event/CaloAdc.h"

namespace Calo { namespace DAQ { namespace TAE {

namespace details {

    enum class source_t { from_adc, from_digit };
    const char* toString(const source_t& source);

    inline std::ostream& toStream(const source_t& source, std::ostream& os) {
        return os << toString(source);
    }
    StatusCode parse(source_t& result, const std::string& input );

}

/** @class CaloMergeTAE CaloMergeTAE.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-10-24
 */
class Merger : public GaudiAlgorithm {
public:
  /// Standard constructor
  Merger( const std::string& name, ISvcLocator* pSvcLocator );

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
  Gaudi::Property<details::source_t> m_data { this ,"OutputType", details::source_t::from_digit };

  Gaudi::Property<std::string> m_outputDataLoc { this, "OutputDataLocation", "" };
  Gaudi::Property<std::string> m_inExt { this, "inputExtension", "" };

  DeCalorimeter* m_calo = nullptr;

};
}}}

using CaloMergeTAE = Calo::DAQ::TAE::Merger;
#endif // CALOMERGETAE_H
