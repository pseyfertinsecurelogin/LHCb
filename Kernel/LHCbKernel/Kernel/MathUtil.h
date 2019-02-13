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

#include <cmath>


/// Determine if value is in given interval.
/// \param value The value to be checked to be in range.
/// \param a One end of the range, can be upper or lower limit.
/// \param b Other end of the range, can be lower or upper limit.
/// \return True if inside interval [a,b], false otherwise.
template <class T>
bool IsInsideInterval(T value, T a, T b) {
    // Looks overcomplicated, but no branches.
    // Copysign and && optimize out to bit-ops.
    T rebasedInterval = a - b;
    T sgn = std::copysign(1.0f, rebasedInterval);
    T rebasedValue = sgn*(value - b);
    rebasedInterval *= sgn;
    return 0.0f <= rebasedValue && rebasedValue <= rebasedInterval;
}



/// Determine if value is in given interval expanded by tolerance.
/// \param value The value to be checked to be in range.
/// \param a One end of the range, can be upper or lower limit.
/// \param b Other end of the range, can be lower or upper limit.
/// \param tolerance The interval is enlarged by tolerance in both directions before checking.
/// \return True if inside interval [a-tolerance,b+tolerance], false otherwise.
template <class T>
bool IsInsideInterval(T value, T a, T b, T tolerance) {
    T rebasedInterval = a - b;
    T sgn = std::copysign(1.0f, rebasedInterval);
    T rebasedValue = sgn*(value - b);
    rebasedInterval *= sgn;
    return -tolerance <= rebasedValue && rebasedValue <= rebasedInterval + tolerance;
}

