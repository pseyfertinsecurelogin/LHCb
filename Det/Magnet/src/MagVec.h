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
#ifndef MAGNET_MAGVEC_H
#define MAGNET_MAGVEC_H





class MagVec {


public:

  MagVec(int N);
  MagVec();
  MagVec(const MagVec& v);
 ~MagVec();


  double& operator[](int i);
  const double& operator[](int i) const;


  MagVec& operator=(const MagVec& v);
  //  MagVec& operator=(const double& v);
  MagVec  operator+(const MagVec& v);
  MagVec  operator+(const MagVec& v) const;
  MagVec& operator+=(const MagVec& v);
  MagVec  operator-(const MagVec& v);
  MagVec  operator-(const MagVec& v) const;
  MagVec& operator-=(const MagVec& v);
  double operator*(const MagVec& v);
  double operator*(const MagVec& v) const;
  MagVec  operator*(const double& d);
  MagVec& operator*=(const double& d);

  void reSize(int Nnew);
  int n_elem() const;


private:



  int Nele;

  double* ptr_data;


};



#endif // MAGNET_MAGVEC_H
