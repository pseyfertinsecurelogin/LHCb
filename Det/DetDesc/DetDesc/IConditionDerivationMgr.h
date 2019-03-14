/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include <GaudiKernel/IInterface.h>
#include <memory>

namespace LHCb::DetDesc {
  class ConditionDerivation;

  /// Interface for managers of ConditionDerivation objects.
  class GAUDI_API IConditionDerivationMgr : virtual public IInterface {
  public:
    /// InterfaceID
    DeclareInterfaceID( IConditionDerivationMgr, 2, 0 );

    /// Identifier of a specific derivation object in the manager.
    using DerivationId = std::size_t;

    /// Add a derivation object to the manager.
    /// After the invocation, the manager will be in control of the lifetime of the
    /// derivation object.
    virtual DerivationId push( std::unique_ptr<ConditionDerivation> derivation ) = 0;

    /// Retire a specific derivation object from the manager.
    virtual std::unique_ptr<ConditionDerivation> pop( DerivationId dId ) = 0;
  };
} // namespace LHCb::DetDesc
