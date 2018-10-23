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
// $Id: Node.cpp,v 1.4 2007-11-30 13:12:29 wouter Exp $

// local
#include "Event/Node.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Node
//
// 2006-06-10 : M. Needham
//-----------------------------------------------------------------------------

LHCb::Node* LHCb::Node::clone() const 
{

  return new LHCb::Node(*this);
        
}

double LHCb::Node::chi2() const
{
  double res = residual() ;
  double err = errResidual() ;
  return res*res/(err*err) ;
}
