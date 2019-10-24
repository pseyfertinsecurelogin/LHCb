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
#ifndef KERNEL_STLOSTREAMREDIRECT_H
#define KERNEL_STLOSTREAMREDIRECT_H 1

#include <cstdio>
#include <iostream>
#include <unistd.h>

namespace STL {

  /** @class OStreamRedirect STLOStreamRedirect.h Kernel/STLOStreamRedirect.h
   *
   *  Simple gaurd like class that redirects all cout and cerr printout
   *  to the given address, as long as the gaurd exists.
   *
   *  @author Chris Jones
   *  @date   2015-09-22
   */
  class OStreamRedirect {

  public:
    /// Constructor. Starts redirection.
    explicit OStreamRedirect( const std::string& loc = "/dev/null" ) {
      original_stdout = dup( fileno( stdout ) );
      fflush( stdout );
      freopen( loc.c_str(), "w", stdout );
      original_stderr = dup( fileno( stderr ) );
      fflush( stderr );
      freopen( loc.c_str(), "w", stderr );
    }

    /// Destructor. Put streams back to normal.
    ~OStreamRedirect() {
      fflush( stdout );
      dup2( original_stdout, fileno( stdout ) );
      close( original_stdout );
      fflush( stderr );
      dup2( original_stderr, fileno( stderr ) );
      close( original_stderr );
    }

  private:
    int original_stdout{0}; ///< Original stdout
    int original_stderr{0}; ///< original stderr
  };

} // namespace STL

#endif // KERNEL_STLOSTREAMREDIRECT_H
