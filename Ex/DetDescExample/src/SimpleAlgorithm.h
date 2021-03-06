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
#ifndef DDEXAMPLE_SIMPLEALGORITHM_H
#define DDEXAMPLE_SIMPLEALGORITHM_H 1

#include "DetDesc/ILVolume.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class SimpleAlgorithm SimpleAlgorithm.h
 *  Example of an algorithm using the detector data service
 *  and the declaration of specific detertor element converter
 *
 *  @author Sebastien Ponce
 *  @date   2002-04-29
 */

class SimpleAlgorithm : public GaudiAlgorithm {
public:
  /**
   * Constructor: A constructor of this form must be provided.
   */
  SimpleAlgorithm( const std::string& name, ISvcLocator* pSvcLocator );

  /**
   * The "initialization" of the algorithm.
   * It creates the environment needed for processing the events.
   * In this example, everything is done here since no events are
   * processed.
   */
  StatusCode initialize() override;

  /**
   * This is were the code for the event processing should reside.
   * In this example, we do nothing here since we don't process any event.
   */
  StatusCode execute() override;

  /**
   * We use this place to display the status of the transient data store.
   */
  StatusCode finalize() override;

  /**
   * Displays the list of existing physical volumes in lv
   * @param lv the logical volume concerned
   * @param de the name of the detector element associated to lv
   */
  void dumpPVs( const ILVolume* lv, std::string de );
};
#endif // DDEXAMPLE_SIMPLEALGORITHM_H
