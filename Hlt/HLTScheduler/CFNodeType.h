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
#pragma once

#include <map>
#include <string>
#include <vector>

// types of CompositeNodes, extendable if needed
enum class nodeType { LAZY_AND, NONLAZY_OR , NONLAZY_AND, LAZY_OR, NOT };

// TODO this duplicates CompositeNode::getType
std::map<nodeType, std::string> const nodeTypeNames = {
    {nodeType::LAZY_AND, "LAZY_AND"},
    {nodeType::NONLAZY_OR, "NONLAZY_OR"},
    {nodeType::NONLAZY_AND, "NONLAZY_AND"},
    {nodeType::LAZY_OR, "LAZY_OR"},
    {nodeType::NOT, "NOT"},
};

std::map<std::string, nodeType> const nodeTypeNames_inv = {
    { "LAZY_AND", nodeType::LAZY_AND },
    { "NONLAZY_OR", nodeType::NONLAZY_OR },
    { "NONLAZY_AND", nodeType::NONLAZY_AND },
    { "LAZY_OR", nodeType::LAZY_OR },
    { "NOT", nodeType::NOT },
};

struct NodeDefinition final {
  std::string name;
  std::string type;
  std::vector<std::string> children;
  bool ordered;
};
