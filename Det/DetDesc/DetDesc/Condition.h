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
#ifndef DETDESC_CONDITION_H
#define DETDESC_CONDITION_H 1

#include <string_view>
#include <string>

// Base classes
#include "DetDesc/ParamValidDataObject.h"

// Unique ID of the class
#include "DetDesc/CLIDCondition.h"

/** @class Condition Condition.h DetDesc/Condition.h
 *
 *  A DataObject implementing the IParamSet, IValidity and
 *  ICondition interfaces.
 *  This is the basic type of condition data in the transient data store
 *  (alignment, slow control, etc.).
 *
 *  @author Sebastien Ponce
 *  @author Andrea Valassi
 */
struct Condition : ParamValidDataObject {

  /// Default constructor
  Condition() = default;

  /// Update using another ValidDataObject: deep copy all contents,
  /// except for the properties of a generic DataObject
  using ParamValidDataObject::update;
  void update ( ValidDataObject& obj) override;

  /// Prepare an XML string representing the condition.
  virtual std::string toXml(std::string_view name = {}, bool header = false, int precision = 16) const;

  /**
   * This method initializes the condition. It should be overridden
   * and used for computation purposes. This is a kind of hook for adding
   * user code easily in the initialization of a condition.
   */
  virtual StatusCode initialize();

  // Re-implemented from DataObject

  /// Class ID of this instance
  inline const CLID& clID() const override { return classID(); }

  /// Class ID of this class
  inline static  const CLID& classID() { return CLID_Condition; }

};

#endif // DETDESC_CONDITION_H
