/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef IFUTUREGAMMAPI0SEPARATIONTOOL_H
#define IFUTUREGAMMAPI0SEPARATIONTOOL_H 1

// Include files
// from STL
#include <map>

// from Gaudi
#include "CaloFutureInterfaces/Enums.h"
#include "Event/CaloHypo.h"
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/STLExtensions.h"

/** @class IFutureGammaPi0SeparationTool IFutureGammaPi0SeparationTool.h
 *
 *
 *  @author Miriam Calvo Gomez --
 *  @date   2019-03-28
 */
namespace Calo::Future::Interfaces {
  struct IGammaPi0Separation : extend_interfaces<IAlgTool> {

    // Return the interface ID
    DeclareInterfaceID( IGammaPi0Separation, 1, 0 );

    virtual double                                  isPhoton( const LHCb::CaloHypo& hypo ) = 0;
    virtual double                                  isPhoton( LHCb::span<const double> )   = 0;
    virtual const std::map<Enum::DataType, double>& inputDataMap()                         = 0;
  };
} // namespace Calo::Future::Interfaces
#endif // IFUTUREGAMMAPI0SEPARATIONTOOL_H
