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
// $Id: CaloFutureUtilsDict.h,v 1.5 2010-03-18 13:58:42 ibelyaev Exp $
// ============================================================================
// CVS Tag $Name: not supported by cvs2svn $ , version $Revision: 1.5 $
// ============================================================================
#ifndef CALOFUTUREUTILS_CALOFUTUREUTILSDICT_H 
#define CALOFUTUREUTILS_CALOFUTUREUTILSDICT_H 1
// ============================================================================
// Include files
// ============================================================================
// CaloFutureUtils 
// ============================================================================
#include "CaloFutureUtils/CaloMomentum.h"
#include "CaloFutureUtils/CaloFutureParticle.h"
#include "CaloFutureUtils/CaloFutureCellIDAsProperty.h"
#include "CaloFutureUtils/CaloFutureUtils.h"
#include "CaloFutureUtils/ICaloFutureElectron.h"
#include "CaloFutureUtils/Kinematics.h"
// ============================================================================
// DigiEvent 
// ============================================================================
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
// ============================================================================
// Stefan Roiser's trick to allow compilation on Win32
// ============================================================================
#ifdef WIN32
#ifdef CONST
#undef CONST
#endif 
#endif
// ============================================================================
namespace
{
  // ==========================================================================
  struct _Instantiations 
  {
    LHCb::CaloCellID::Vector m_1 ;
    LHCb::CaloCellID::Set    m_2 ;
    LHCb::CaloDigit::Vector  m_3 ;
    LHCb::CaloDigit::Set     m_4 ;    
    
    CaloVector<LHCb::CaloDigit>  m_5 ;
    CaloVector<LHCb::CaloDigit*> m_6 ;
    CaloVector<LHCb::CaloAdc>    m_7 ;
  } ;
  //===========================================================================
}
// ============================================================================
// The END 
// ============================================================================
#endif // CALOFUTUREUTILS_CALOFUTUREUTILSDICT_H
// ============================================================================
