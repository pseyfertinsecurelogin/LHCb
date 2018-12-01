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
#ifndef     DETDESC_IPVOLUME_PREDICATES_H
#define     DETDESC_IPVOLUME_PREDICATES_H
// STD & STL
#include <iostream>
#include <functional>
#include <string_view>
// Geometry definitions
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"
// DetDesc
#include "DetDesc/IPVolume.h"
#include "DetDesc/ILVolume.h"

/** @file IPVolume_predicates.h
 *  define useful predicate to deal with IPVolume and ILVolume
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */

/** @class IPVolume_isInside IPVolume_predicates.h
 *
 * This predicate allows search from a sequence of IPVolume for a IPVolume,
 * which contains the given point
 * (container)<IPVolume*> volumes = ...
 * (container)<IPvolume*>::const_ietartor ci =
 * std::find_if( volumes.begin() , volumes.end() ,
 *  IPVolume_isInside(Point) );
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */

  /** explict constructor
   *  @param PointInMotherFrame point in mother reference system
   */
static auto IPVolume_isInside = [](Gaudi::XYZPoint PointInMotherFrame)
{
    return [pnt=std::move(PointInMotherFrame)](const IPVolume* pv) {
        return pv && pv->isInside(pnt);
    };
};


/** @class  IPVolume_byName IPVolume_predicates.h
 *  This predicate allows search from a sequence of
 *  IPVolume for a IPVolume by name
 *    (container)<IPVolume*> volumes = ...
 *  (container)<IPvolume*>::const_ietartor ci =
 * std::find_if( volumes.begin() , volumes.end() ,
 *  IPVolume_byName(name) );
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */

  /** find physical volume by name
   *  @param pv pointer to physical volume
   *  @return true if name matches the given name
   */
// TODO: add overload for std::string instead of string_ref
static auto IPVolume_byName = [](std::string_view name) {
    return [=](const IPVolume* pv)
           { return pv && name == pv->name(); };
};

/** @class IPVolume_accumulateMatrix IPVolume_predicates.h
 *
 * used for std::accumulate algorithm
 * (container)<IPVolume*> volumes = ...
 * std::accumulate(...)
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
static auto IPVolume_accumulateMatrix =
    [](Gaudi::Transform3D& mtrx, const IPVolume* pv) -> decltype(auto)
    { mtrx = pv->matrix()*mtrx; return mtrx; };

/** @class IPVolume_fromReplica IPVolume_predicates.h
 *
 * used for transformation of ILVolume::ReplicaPath into
 * ILVolume::PVolumePath
 * used for "std::transform" algorithm
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
  /** constructor
   *  @param LV pointer to Logical Volume
   */
static auto IPVolume_fromReplica = [](const ILVolume* LV) {
  /** get pointer to physical volume by replica number
   *  @param replica replica number
   *  @return pointer to physical volume
   */
    return [m_lv=LV]
           (const ILVolume::ReplicaType& replica) mutable
           -> const IPVolume* {
        if( !m_lv ) return nullptr;
        const IPVolume* pv = (*m_lv)[replica];
        if( !pv ) { m_lv = nullptr ; return nullptr ; }
        m_lv = pv->lvolume();
        return pv;
    };
};


// ============================================================================
#endif  ///<  DETDESC_IPVOLUME_PREDICATES_H
// ============================================================================
