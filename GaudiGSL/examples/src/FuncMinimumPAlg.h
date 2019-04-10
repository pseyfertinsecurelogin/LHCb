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
#ifndef ALGTOOLS_FUNCMINIMUMPALG_H
#define ALGTOOLS_FUNCMINIMUMPALG_H 1

// Include files

// from Gaudi
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"

/// Forward references
struct IFuncMinimum;

//-----------------------------------------------------------------------------
/**  @class FuncMinimumPAlg FuncMinimumPAlg.h
 *   @file Algorithm file for the class IFuncMinimum
 *   @see GaudiGSL/IFuncMinimum.h
 *   @author Kirill Miklyaev kirillm@iris1.itep.ru
 *   @date   2002-09-14
 */
//-----------------------------------------------------------------------------

class FuncMinimumPAlg : public Algorithm {

public:
  using Algorithm::Algorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  IFuncMinimum* m_privateTool;
  IFuncMinimum* m_publicTool;
};
#endif // ALGTOOLS_FUNCMINIMUMPALG_H
