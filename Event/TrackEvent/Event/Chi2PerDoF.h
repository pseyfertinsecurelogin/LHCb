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

#pragma once


namespace LHCb {

    struct Chi2PerDoF {
        float chi2PerDoF = 0.0f; ///< Total chi2 divided by the number of degrees of freedom.
        int nDoF = 0; ///< Number of degrees of freedom.
        float chi2() const { return chi2PerDoF*nDoF; }
    };

} // namespace LHCb