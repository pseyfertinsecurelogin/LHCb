/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef KERNEL_DIFTRAJ_H
#define KERNEL_DIFTRAJ_H 1

// Include files

// local
#include "Kernel/Trajectory.h"

/** @class DifTraj DifTraj.h Kernel/DifTraj.h
 *
 *  DifTraj is a templated class inheriting from Trajectory.h.
 *  It interfaces the derivative and parameters methods, which
 *  can have different dimensions for different XxxTraj classes.
 *
 *  @author Edwin Bos
 *  @date   2006-02-03
 */

namespace LHCb
{

  template<unsigned int N>
  struct DifTraj : Trajectory<double> {

    enum { kSize = N };

    /// Constructor - defer to Trajectory
    using Trajectory<double>::Trajectory;
    using Trajectory<double>::Vector;
    using Trajectory<double>::Point;

    typedef ROOT::Math::SMatrix<double,3,N> Derivative;
    typedef ROOT::Math::SVector<double,N>   Parameters;

    /// Retrieve the derivative of the point at fixed expansion parameter 'mu'
    /// with respect to the parameters used to describe the trajectory
    virtual Derivative derivative( double mu ) const = 0;

    /// Retrieve the parameters of the DifTraj -- these should be
    /// the values of the parameters with respect to which 'derivative'
    /// provides the derivatives...
    virtual Parameters parameters() const = 0;

    // update the parameters i.e.
    //   parameters += delta...
    virtual DifTraj<N>& operator+=(const Parameters& delta) = 0;
  };

} // namespace LHCb

#endif // KERNEL_DIFTRAJ_H
