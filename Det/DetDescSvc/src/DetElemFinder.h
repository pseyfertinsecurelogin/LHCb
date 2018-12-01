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
#ifndef DETELEMFINDER_H
#define DETELEMFINDER_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "GaudiKernel/HashMap.h"
#include "DetDesc/IDetElemFinder.h"

/** @class DetElemFinder DetElemFinder.h
 *
 *  Implementation of IDetElemFinder.
 *
 *  @author Marco Clemencic
 *  @date   2006-09-01
 */
class DetElemFinder: public extends<Service,IDetElemFinder> {
public:

  /// Standard constructor
  using base_class::base_class;

  /** Query interfaces (\see{IInterface})
      @param riid       ID of Interface to be retrieved
      @param ppvUnknown Pointer to Location for interface pointer
  */

  /// Find the detector element instance associated to a given physical volume path.
  /// Returns 0 if there is no suitable detector element.
  const IDetectorElement * detectorElementForPath(const std::string &path) const override;

  /// Initialize Service
  StatusCode initialize() override;

  /// Finalize Service
  StatusCode finalize() override;


private:

  /// Recursively add the given detector element and its children.
  /// The parameter parent_path is the used to speed up recursion.
  StatusCode insert(const IDetectorElement *de, const std::string &parent_path = "");

  /// Find the detector element path and fill the variable path.
  /// If parent_path is not specified, it retrieved recursively.
  StatusCode detector_element_path(const IDetectorElement *de, std::string &path,
                                   const std::string &parent_path = "") const;

  /// Name of the Data Provider (set by the option DetDataSvc, by default "DetectorDataSvc").
  Gaudi::Property<std::string> m_detDataSvcName { this, "DetectorDataSvc", "DetectorDataSvc" };

  /// Path to the detector element to start from. (DetElemFinder.RootElement = "/dd/Structure/LHCb")
  Gaudi::Property<std::string> m_rootElement { this, "RootElement", "/dd/Structure/LHCb" };

  /// Flag to dump the retrieved map of detector elements. (DetElemFinder.DumpMap = false)
  Gaudi::Property<bool> m_dumpMap { this,  "DumpMap",  false };

  /// Structure for the mapping
  GaudiUtils::HashMap<std::string, const IDetectorElement *> m_map;

};
#endif // DETELEMFINDER_H
