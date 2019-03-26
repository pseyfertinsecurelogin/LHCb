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

//--------------------------------------------------------------------------------
/** @file RichSmartIDCnv.cpp
 *
 *  Implementation file for RichSmartID conversion utilities
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   05/02/2008
 *  @edits: Young Min Kim (s0679231@sms.ed.ac.uk) on 13/06/2008
 */
//--------------------------------------------------------------------------------

// local
#include "RichUtils/RichSmartIDCnv.h"

MsgStream& Rich::SmartIDGlobalOrdering::fillStream( MsgStream& os ) const {
  return os << "["
            << " pdX=" << globalPdX() << " pdY=" << globalPdY() << " pixX=" << globalPixelX()
            << " pixY=" << globalPixelY() << " ]";
}
