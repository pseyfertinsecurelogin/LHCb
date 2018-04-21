#ifndef MUONDET_MUONCHAMBERGRID_H
#define MUONDET_MUONCHAMBERGRID_H 1

#include "DetDesc/Condition.h"

// Include files
#include "MuonDet/MuonFrontEndID.h"

/** @class MuonChamberGrid MuonChamberGrid.h MuonDet/MuonChamberGrid.h
 *
 *
 *  @author Alessia(o) Sat(r)ta(i)
 *  @date   2004-01-07
 */

/// Class ID of the Muon Grid
static const CLID CLID_MuonChamberGrid = 11094;

class MuonChamberGrid: public Condition {
public:
  /// Standard constructor
  MuonChamberGrid( ) = default;

  StatusCode initialize() override;

  //Returns distances relative to chamber dimension
  std::vector< std::pair<MuonFrontEndID, std::array<float,4> > > listOfPhysChannels(double x_enter,double y_enter,
										   double x_exit,double y_exit);

  double retLenght(int nLx,  const std::vector<double>& my_list);

  // Class ID of this class
  static  const CLID& classID() { return CLID_MuonChamberGrid; };

  unsigned int getNGrid(){ return m_number_of_grid; }

  const std::vector<int>& getReadoutGrid() { return m_readoutType; }

  const std::vector<int>& getMapGrid() { return m_mapType; }

  int getGrid1SizeX(){ return m_x_pad_rdout1.size(); }

  int getGrid1SizeY(){ return m_y_pad_rdout1.size(); }

  int getGrid2SizeX(){ return m_x_pad_rdout2.size(); }

  int getGrid2SizeY(){ return m_y_pad_rdout2.size(); }


  StatusCode getPCCenter(MuonFrontEndID fe,
                         double& xcenter, double& ycenter);
private:

  int m_number_of_grid = 0;
  std::vector<int> m_readoutType;
  std::vector<int> m_mapType;
  std::vector<double> m_x_pad_rdout1;
  std::vector<double> m_y_pad_rdout1;
  std::vector<double> m_x_pad_rdout2;
  std::vector<double> m_y_pad_rdout2;

};
#endif // MUONDET_MUONCHAMBERGRID_H

