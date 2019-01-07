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
/// ===========================================================================
#ifndef DETDESC_SOLID_H
#define DETDESC_SOLID_H 1
/// DetDesc
#include "DetDesc/ISolid.h"

/** @namespace Solid Solid.h "DetDesc/Solid.h"
 *
 *  A number of useful methods to deal with solids
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   03/08/2001
 */

namespace Solid
{
  /** @class IsInside Solid.h "DetDesc/Solid.h"
   *
   *  useful class to check if point is inside of
   *  the given solid
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date 03/08/2001
   */

  template <typename Point>
  auto isInside( Point&& p ) {
      return [point=std::forward<Point>(p)](const ISolid& solid)
             { return solid.isInside(point); };
  }

} ///< end of namespace Solid

/// ===========================================================================
#endif ///< DETDESC_SOLID_H
