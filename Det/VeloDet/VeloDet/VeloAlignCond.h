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
#ifndef VELODET_VELOALIGNCOND_H
#define VELODET_VELOALIGNCOND_H 1


// Include files
#include <mutex>
#include "GaudiKernel/MsgStream.h"
#include "DetDesc/AlignmentCondition.h"
#include "VeloDet/CLIDVeloAlignCond.h"
/** @class VeloAlignCond VeloAlignCond.h VeloDet/VeloAlignCond.h
 *
 *
 *  @author Marco Clemencic
 *  @date   2008-06-23
 */

class IMessageSvc;

class VeloAlignCond : public AlignmentCondition {

public:
  /**
   * Helper class to handle the two strings indicating the location in the TS
   * of the stepping motors readings.
   */
  struct PositionPaths {
    typedef std::pair<std::string,std::string> ValueType;
    PositionPaths(): x(), y() {}
    PositionPaths(const std::string &_x,const std::string &_y);
    ValueType x,y;
    bool operator!=(const PositionPaths &rhs) const {
      return x != rhs.x || y != rhs.y;
    }
  };

  /// Standard constructor
  VeloAlignCond( ) = default;
  /// Constructor
  VeloAlignCond(const std::vector<double>& translation,
                const std::vector<double>& rotation,
                const std::vector<double>& pivot = std::vector<double>(3, 0.),
                const std::string &xOffsetLoc = "",
                const std::string &yOffsetLoc = "") ;

  virtual ~VeloAlignCond(); ///< Destructor

  /// Initialize the internal structures (transformation matrices)
  StatusCode initialize() override;

  using AlignmentCondition::update;
  /// Update internal data from another condition.
  void update(ValidDataObject& obj) override;

  /// Class ID of this instance
  inline const CLID& clID() const override { return classID(); }
  /// Class ID of this class
  inline static  const CLID& classID() { return CLID_VeloAlignCond; }

  /// Creates the transformation from the motion system
  Gaudi::Transform3D motionSystemTransform() const ;

protected:

  /// @see AlignmentCondition
  StatusCode makeMatrices() override;

  /// @see AlignmentCondition
  void updateParams(const Gaudi::Transform3D& matrixInv) override;

  /// Commodity function to register to the UpdateMgrSvc for an offset condition.
  virtual void i_registerOffsetCond(const PositionPaths::ValueType &offsetCond, Condition *&cond,
                                    PositionPaths::ValueType &oldOffsetCond);

  /// Variable to store the location and parameters of the offset conditions.
  PositionPaths m_paths;

  /// Pointer to the condition used for the offset along x (MotionSystem).
  Condition *m_xOffCond = nullptr;
  /// Pointer to the condition used for the offset along y (MotionSystem).
  Condition *m_yOffCond = nullptr;

  /// Flag to remember if the condition registered itself to the UpdateManagerSvc.
  bool m_inUpdMgrSvc = false;

private:

  /// Thread safe on demand access to MsgStream object
  inline MsgStream & msg() const {
    std::call_once(m_msgSetFlag,
		   [&]{m_msgStream.reset( new MsgStream( msgSvc(), "VeloAlignCond" ) );});
    return *m_msgStream;
  }
  /// cached Message Stream object
  mutable std::unique_ptr<MsgStream> m_msgStream;
  /// making the msg() function above thread safe
  mutable std::once_flag m_msgSetFlag;


};

#endif // VELODET_VELOALIGNCOND_H
