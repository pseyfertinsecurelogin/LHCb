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
// ===========================================================================
#pragma once
/// STD and STL
#include <cmath>
#include <iostream>
/// DetDesc
#include "DetDesc/SolidBase.h"
#include "DetDesc/SolidPolyHedronHelper.h"
/// forward declarations
template <class TYPE>
class SolidFactory;

/** @class SolidTrd SolidTrd.h  "DetDesc/SolidTrd.h"
 *
 *   A simple implementation of TRD
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date xx/xx/xxxx
 */

class SolidTrd final : public virtual SolidBase, protected virtual SolidPolyHedronHelper {
  /// friend factory for instantiation
  friend class SolidFactory<SolidTrd>;

public:
  /** constructor
   *  @param name name of the solid object
   *  @param zHalfLength half-length in z-direction
   *  @param XHalfLength1 half-length in x-direction at z1
   *  @param YHalfLength1 half-length in y-direction at z1
   *  @param XHalfLength2 half-length in x-direction at z2
   *  @param YHalfLength2 half-length in y-direction at z2
   *  @exception SolidException wrong parameter range
   */
  SolidTrd( const std::string& name,         //
            const double       zHalfLength,  //
            const double       XHalfLength1, //
            const double       YHalfLength1, //
            const double       XHalfLength2, //
            const double       YHalfLength2 );

  /** - retrieve the TRD type
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  return box type
   */
  inline std::string typeName() const override { return "SolidTrd"; }

  /** - retrieve the pointer to "simplified" solid - "cover"
   *    -\# The cover for general TRD is "symmetric" TRD
   *    -\# the cover for symmetric TRD is BOX
   *  - implementation of ISolid abstract interface
   *  @see ISolid
   *  @return pointer to "simplified" solid - "cover"
   */
  inline const ISolid* cover() const override { return m_cover.get(); }

  /** - printout to STD/STL stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( std::ostream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut( std::ostream& os = std::cout ) const override;

  /** - printout to Gaudi MsgStream stream
   *  - implementation  of ISolid abstract interface
   *  - reimplementation of SolidBase::printOut( MsgStream& )
   *  @see SolidBase
   *  @see ISolid
   *  @param os  Gaudi MsgStream stream
   *  @return reference to the stream
   */
  MsgStream& printOut( MsgStream& os ) const override;

  /**  half size in x at point 1
   *  @return half size in x at point 1
   */
  inline double xHalfLength1() const noexcept { return m_trd_xHalfLength1; }

  /**  half size in x at point 2
   *  @return  half size in x at point 2
   */
  inline double xHalfLength2() const noexcept { return m_trd_xHalfLength2; }

  /**  half size in y at point 1
   *  @return half size in y at point 1
   */
  inline double yHalfLength1() const noexcept { return m_trd_yHalfLength1; }

  /**  half size in y at point 2
   *  @return half size in y at point 2
   */
  inline double yHalfLength2() const noexcept { return m_trd_yHalfLength2; }

  /**  half size in z
   *  @return  half size in z
   */
  inline double zHalfLength() const noexcept { return m_trd_zHalfLength; }

  /**  full size in x at point 1
   *  @return full size in x at point 1
   */
  inline double xLength1() const noexcept { return m_trd_xHalfLength1 * 2; }

  /**  full size in x at point 2
   *  @return full size in x at point 2
   */
  inline double xLength2() const noexcept { return m_trd_xHalfLength2 * 2; }

  /**  full size in y at point 1
   *  @return full size in y at point 1
   */
  inline double yLength1() const noexcept { return m_trd_yHalfLength1 * 2; }

  /**  full size in y at point 2
   *  @return full size in y at point 2
   */
  inline double yLength2() const noexcept { return m_trd_yHalfLength2 * 2; }

  /**  full size in z
   *  @return full size in z
   */
  inline double zLength() const noexcept { return m_trd_zHalfLength * 2; }

  //
protected:
  /** constructor
   *  @param name name of trd object
   */
  SolidTrd( const std::string& name = "Anonymous TRD" );
  ///
private:
  /** initialize the polihedron base
   *  @exception SolidException wrong parameters
   */
  void makeAll();

  /// creation of cover
  void createCover();

private:
  SolidTrd( const SolidTrd& );            // no copy-constructor
  SolidTrd& operator=( const SolidTrd& ); // no assignment

private:
  /** data members - TRD parameters  */
  double m_trd_zHalfLength{10000000};
  double m_trd_xHalfLength1{10000};
  double m_trd_xHalfLength2{10000};
  double m_trd_yHalfLength1{10000};
  double m_trd_yHalfLength2{10000};
  /// cover
  std::unique_ptr<ISolid> m_cover;
};

/// ===========================================================================
