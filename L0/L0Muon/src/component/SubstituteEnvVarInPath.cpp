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
#include "SubstituteEnvVarInPath.h"
#include "GaudiKernel/System.h"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

namespace L0MuonUtils {

std::string SubstituteEnvVarInPath(const std::string& in) {
    boost::filesystem::path path(in);
    boost::filesystem::path::iterator i = path.begin();
    boost::filesystem::path out;
    while (i != path.end()) {
       if ( *(i->c_str())=='$' ) {
            std::string x = System::getEnv( i->c_str()+1 );
            out /= x.empty() ? *i : x ;
       } else {
            out /= *i;
       }
       ++i;
    }
    return out.string();
}

}
