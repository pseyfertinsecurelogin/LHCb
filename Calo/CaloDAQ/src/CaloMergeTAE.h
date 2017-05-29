#ifndef CALOMERGETAE_H
#define CALOMERGETAE_H 1

#include <ctype.h>
#include <algorithm>
#include <string>
#include <vector>

#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class DeCalorimeter;
class ISvcLocator;


/** @class CaloMergeTAE CaloMergeTAE.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-10-24
 */
class CaloMergeTAE : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloMergeTAE( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~CaloMergeTAE( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

private:
  bool fromAdc(){
    std::string out( m_data );
    std::transform( m_data.begin() , m_data.end() , out.begin () , ::toupper ) ;
    if( out == "ADC" || out == "CALOADC" || out == "ADCS" || out == "CALOADCS")return true;
    return false;
  }
  bool fromDigit(){
    std::string out( m_data );
    std::transform( m_data.begin() , m_data.end() , out.begin () , ::toupper ) ;
    if( out == "DIGIT" || out == "CALODIGIT" || out == "DIGITS" || out == "CALODIGITS")return true;
    return false;
  }
  void mergeDigits();
  void mergeAdcs();
  std::vector<std::string> m_slots;
  std::string m_detectorName;
  double m_threshold;
  double m_slotcut;
  std::string m_locDigit;
  std::string m_outDigit;
  std::string m_locAdc;
  std::string m_outAdc;
  std::string m_data;

  std::string m_outputDataLoc;
  std::string m_inExt;

  DeCalorimeter* m_calo;

};
#endif // CALOMERGETAE_H
