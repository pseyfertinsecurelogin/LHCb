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
#ifndef RELATIONS_IRELATIONWEIGHTED2D_H
#define RELATIONS_IRELATIONWEIGHTED2D_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
/// local
// ============================================================================
#include "Relations/IRelationWeighted.h"
#include "Relations/RelationUtils.h"
#include "Relations/RelationWeightedTypeTraits.h"
// ============================================================================
/** @class IRelationWeighted2D IRelationWeighted2D.h
 *
 *  An absract interface for bidirectional templated weighted relations
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   23/03/2002
 */
template <class FROM, class TO, class WEIGHT>
class IRelationWeighted2D : public IRelationWeighted<FROM, TO, WEIGHT> {
public:
  // ==========================================================================
  /// shortcut to own type
  typedef IRelationWeighted2D<FROM, TO, WEIGHT> OwnType;
  /// shortcut to inverse type
  typedef IRelationWeighted2D<TO, FROM, WEIGHT> InvType;
  /// shortcut to "direct" type
  typedef IRelationWeighted<FROM, TO, WEIGHT> DirectType;
  /// shortcut to "inverse" type
  typedef IRelationWeighted<TO, FROM, WEIGHT> InverseType;
  // ==========================================================================
public:
  // ==========================================================================
  /** get the "direct" interface
   *  @return pointer to the 'direct' interface
   */
  virtual DirectType* direct() = 0;
  /** get the "direct" interface  (const-version)
   *  @return pointer to the 'direct' interface
   */
  virtual const DirectType* direct() const = 0;
  /** get the "inverse" interface
   *  @return pointer to the 'inverse' interface
   */
  virtual InverseType* inverse() = 0;
  /** get the "inverse" interface  (const version)
   *  @return pointer to the 'inverse' interface
   */
  virtual const InverseType* inverse() const = 0;
  // ==========================================================================
public:
  // ==========================================================================
  /** interface identification (static)
   *  @attention the unique interface identifier is constructed "on-fly"
   *  using hash-technique from the generic interface name and
   *  unique identifiers of related objects
   *  @see IInterface
   *  @return the uniqie interface identifier
   */
  static const InterfaceID& interfaceID() {
    static const InterfaceID s_iid = Relations::interfaceID( System::typeinfoName( typeid( OwnType ) ) );
    return s_iid;
  }
  // ==========================================================================
protected:
  // ==========================================================================
  /// destructor (virtual and protected)
  virtual ~IRelationWeighted2D() = default;
  // ==========================================================================
};
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_IRELATIONWEIGHTED2D_H
