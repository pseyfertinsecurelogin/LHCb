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
#ifndef DICT_HLTINTERFACESDICT_H
#define DICT_HLTINTERFACESDICT_H 1

#include "Kernel/IANNSvc.h"
#include "Kernel/IANSvc.h"
#include "Kernel/ConfigTreeNodeAlias.h"
#include "Kernel/ConfigTreeNode.h"
#include "Kernel/PropertyConfig.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/IConfigAccessSvc.h"
#include "Kernel/IConfigTreeEditor.h"
#include "Kernel/ReadRoutingBits.h"
#include "Kernel/IHltMonitorSvc.h"
#include "Kernel/ILinePersistenceSvc.h"

namespace {

  struct _hltinterfaces_instantiations
  {
    std::vector<ConfigTreeNodeAlias> py_hlt_07;
    std::optional<ConfigTreeNode> py_hlt_08;
    std::optional<PropertyConfig> py_hlt_09;
    _hltinterfaces_instantiations(){}
  };
}


#endif // DICT_HLTINTERFACESDICT_H
