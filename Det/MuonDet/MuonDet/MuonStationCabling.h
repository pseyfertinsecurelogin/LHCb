#ifndef MUONDET_MUONSTATIONCABLING_H
#define MUONDET_MUONSTATIONCABLING_H 1

#include <vector>
#include "GaudiKernel/DataObject.h"
#include "DetDesc/Condition.h"

// Include files
#include "MuonDet/CLID_MuonStationCabling.h"
/** @class MuonStationCabling MuonStationCabling.h MuonDet/MuonStationCabling.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-07
 */
class MuonStationCabling: public Condition {
public:
  /// Standard constructor
  MuonStationCabling( ) = default;

  using Condition::update;
  virtual void update ( Condition& obj );
  void update ( ValidDataObject& obj ) override;
  StatusCode update(long l1numb);

  /// Class ID of this class
  inline static  const CLID& classID() {
    return CLID_MuonStationCabling;
  }

  long getNumberOfL1Board() const {return m_numberOfL1Board;}
  const std::string& getL1Name(unsigned int i) const {return m_listOfL1[i];}
  const std::vector<std::string>& getAllL1Names() const {return m_listOfL1;}
  StatusCode addL1Name(std::string name);

private:
  long m_numberOfL1Board = 0;
  std::vector<std::string> m_listOfL1;

};
#endif // MUONDET_MUONSTATIONCABLING_H
