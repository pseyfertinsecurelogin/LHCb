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

#include "MDF/Buffer.h"

#include "Kernel/STLExtensions.h"

#include <cstddef>
#include <memory>
#include <string>

#include <sys/mman.h>

namespace LHCb::MDF {

  /**
   * helper class handling a mapped file and its corresponding buffer
   */
  class MMappedFile {
  public:
    /**
     * this constructor maps the given file to memory before returning
     * @throws GaudiException in case the file is not found or cannot be mounted
     */
    MMappedFile( std::string const& fileName );
    ~MMappedFile();
    MMappedFile( const MMappedFile& ) = delete;
    MMappedFile( MMappedFile&& )      = delete;
    LHCb::span<std::byte> const data() { return m_data; }
    std::byte*                  end() { return m_data.data() + m_data.size(); }

  private:
    LHCb::span<std::byte> m_data;
  };

  using MmapBuffer = LHCb::MDF::OwningBuffer<std::shared_ptr<MMappedFile>>;

} // namespace LHCb::MDF
