/***************************************************************************** \
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#include "MMappedFile.h"

#include "GaudiKernel/GaudiException.h"

#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

LHCb::MDF::MMappedFile::MMappedFile( std::string const& fileName ) {
  // get file size
  struct stat st;
  int         rc = stat( fileName.c_str(), &st );
  if ( rc == -1 ) { throw GaudiException( "Could not find input : " + fileName, "MMAppedFile", StatusCode::FAILURE ); }
  // open file
  int fd = open( fileName.c_str(), O_RDONLY, 0 );
  if ( fd == -1 ) {
    std::stringstream s;
    s << "Could not open input : " << fileName << ", errno = " << errno;
    throw GaudiException( s.str(), "MMAppedFile", StatusCode::FAILURE );
  }
  // map file
  auto rawdata = mmap( NULL, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0 );
  if ( rawdata == MAP_FAILED ) {
    std::stringstream s;
    s << "Could not map input : " << fileName << ", errno = " << errno;
    close( fd );
    throw GaudiException( s.str(), "MMAppedFile", StatusCode::FAILURE );
  }
  m_data = LHCb::span<std::byte>{static_cast<std::byte*>( rawdata ), st.st_size};
  // close file descriptor
  if ( 0 != close( fd ) ) {
    munmap( m_data.data(), m_data.size() );
    throw GaudiException( "Could not close input after mmap : " + fileName, "MMAppedFile", StatusCode::FAILURE );
  }
}

LHCb::MDF::MMappedFile::~MMappedFile() {
  // unmap memory
  munmap( m_data.data(), m_data.size() );
}
