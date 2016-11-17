#ifndef CALOENERGYFROMRAW_H
#define CALOENERGYFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloDAQ/ICaloEnergyFromRaw.h"            // Interface
#include "CaloReadoutTool.h"

/** @class CaloEnergyFromRaw CaloEnergyFromRaw.h
 *  Decode the calorimeter energies, accodrding to version
 *
 *  @author Olivier Callot
 *  @date   2005-01-10
 */
class CaloEnergyFromRaw : public extends<CaloReadoutTool, ICaloEnergyFromRaw>  {
public:
  /// Standard constructor
  CaloEnergyFromRaw( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode initialize() override;

  const std::vector<LHCb::CaloAdc>& adcs( ) override;// decode all banks
  const std::vector<LHCb::CaloAdc>& adcs( const LHCb::RawBank& bank ) override; // decode a single bank
  const std::vector<LHCb::CaloAdc>& adcs( int source ) override; // decode a single Tell1 bank
  const std::vector<LHCb::CaloAdc>& pinAdcs( ) override;
  //
  const std::vector<LHCb::CaloDigit>& digits( ) override; // decode all banks
  const std::vector<LHCb::CaloDigit>& digits( const LHCb::RawBank& bank ) override;// decode a single bank
  const std::vector<LHCb::CaloDigit>& digits( int source ) override;// decode a single Tell1 bank
  void clear() override;
  void cleanData(int feb) override;

private:
  bool getData ( const LHCb::RawBank& bank );
  bool getDigits ();

  std::vector<LHCb::CaloAdc>    m_pinData;
  std::vector<LHCb::CaloAdc>    m_data;
  std::vector<LHCb::CaloDigit>  m_digits;
};
#endif // CALOENERGYFROMRAW_H
