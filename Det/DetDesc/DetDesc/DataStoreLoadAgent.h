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
#ifndef DATASTORELOADAGENT_H
#define DATASTORELOADAGENT_H 1

// Include files
#include "GaudiKernel/IDataStoreAgent.h"

/** @class DataStoreLoadAgent DataStoreLoadAgent.h
 *
 *  Simple class that load all the objects od a transient store.
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-02-24
 */
class DataStoreLoadAgent: public IDataStoreAgent {
public:
  /// Standard constructor
  DataStoreLoadAgent( ) = default;

  bool 	analyse (IRegistry *pObject, int level) override;

  /**
   * Give the number of objects that were loaded.
   */
  inline int loadedObjects () const { return m_loadedObjects; }

  /**
   * Give the maximum depth reached navigating the tree.
   */
  inline int maxDepth () const { return m_maxDepth; }

  /**
   * Reset the counters. Useful when using the same instance many times.
   */

  inline void resetCounters() { m_loadedObjects = m_maxDepth = 0; }


protected:

private:
  int m_loadedObjects = 0;
  int m_maxDepth = 0;
};
#endif // DATASTORELOADAGENT_H
