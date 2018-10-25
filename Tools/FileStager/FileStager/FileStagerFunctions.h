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
// $Id: $
#ifndef FILESTAGER_FILESTAGERFUNCTIONS_H
#define FILESTAGER_FILESTAGERFUNCTIONS_H 1

#include <string>

namespace FileStager {
   bool createPFN( std::string& remote, std::string& command, bool stageLocal = false );
}

#endif // FILESTAGER_FILESTAGERFUNCTIONS_H
