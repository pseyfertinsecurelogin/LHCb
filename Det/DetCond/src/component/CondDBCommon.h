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
#ifndef CONDDBCOMMON_H_
#define CONDDBCOMMON_H_

#include <string>
#include <vector>
#include "DetCond/ICondDBReader.h"

// forward declaration
namespace cool{
  class RecordSpecification;
}

/** @file Utility functions shared among DetCond components.
 *
 * @author Marco Clemencic
 */
namespace CondDB {
  void generateXMLCatalog(const std::string &name,
                          const std::vector<std::string> &fldrs,
                          const std::vector<std::string> &fldrsets,
                          std::string &data);

  StatusCode generateXMLCatalog(ICondDBReader *reader, const std::string &path,
                                ICondDBReader::DataPtr &data);

  const cool::RecordSpecification& getXMLStorageSpec();
}

#endif /*CONDDBCOMMON_H_*/
