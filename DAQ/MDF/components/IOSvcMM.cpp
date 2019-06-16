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

#include "MDF/Buffer.h"
#include "MDF/IOSvc.h"
#include "MDF/MDFHeader.h"
#include "MDF/RawDataConnection.h"

#include <mutex>
#include <sys/mman.h>

namespace {

  using SharedMMappedFile = std::shared_ptr<LHCb::MDF::MMappedFile>;
  using MMapBuffer        = LHCb::MDF::OwningBuffer<SharedMMappedFile>;

  /**
   * helper class dealing with a set of input files and able to read/seek from them
   */
  class InputHandler {
  public:
    struct EndOfFile {};

    /// constructor
    /// connects to first input already and thus may throw EndOfInput
    InputHandler( LHCb::MDF::IOSvc* ioSvc, std::vector<std::string> const& input )
        : m_ioSvc( ioSvc ), m_input( input ) {
      m_curMappedFile = connectToCurrentInput();
      m_curPtr        = m_curMappedFile->data().data();
    }

    /// returns current input's name
    std::string const& inputName() { return m_input[m_curInput]; }
    /**
     * skips next n events without reading them (only headers will be actually read)
     * @throws IIOSvc::EndOfInput
     */
    void skip( unsigned int );
    /**
     * skips current header, according to its size provided via header
     */
    void skip( LHCb::MDFHeader& header ) { m_curPtr += header.recordSize(); }
    /**
     * returns pointer to next Event header
     * @throws IIOSvc::EndOfInput, EndOfFile
     */
    LHCb::MDFHeader* readNextEventHeader();
    /// accessor to currently mapped file
    SharedMMappedFile curMappedFile() { return m_curMappedFile; }

  private:
    /**
     * connects to current input and returns the mmapped buffer containing the data
     * @throws IIOSvc::EndOfInput
     */
    SharedMMappedFile connectToCurrentInput();

  private:
    /// pointer to IOSvc
    LHCb::MDF::IOSvc* m_ioSvc;
    /// vector of input files
    std::vector<std::string> const& m_input;
    /// currently used input as an index in m_input
    unsigned int m_curInput{0};
    /// current pointer in the mapped file
    std::byte* m_curPtr{nullptr};
    /// currently mapped file
    SharedMMappedFile m_curMappedFile;
  };

  /**
   * prefetches events from current mapped file, up to the given number
   * in case the current mapped files does not contain enough events, only prefetches from that file anyway
   * @param bufferSize the size of the buffer to allocate
   */
  std::shared_ptr<LHCb::MDF::Buffer> prefetchEvents( InputHandler& input, unsigned int nbEvents ) {
    // prepare a vector to host events and reserve space
    std::vector<LHCb::MDF::MDFEvent> events;
    events.reserve( nbEvents );
    // get hold of current mapped files
    auto mmapBuffer = input.curMappedFile();
    // Fill buffer with banks while there is enough space and create associated events
    try {
      for ( unsigned int i = 0; i < nbEvents; i++ ) {
        auto* header = input.readNextEventHeader();
        // now let's build the event and copy the raw banks
        events.emplace_back( LHCb::span<std::byte>{(std::byte*)header->data(), header->size()} );
        input.skip( *header );
      }
    } catch ( InputHandler::EndOfFile& e ) {
      // we've reached the end of the current mapped file, stop here
    } catch ( LHCb::IIOSvc::EndOfInput& e ) {
      // we've reached the end of the input
      // if we have no data rethrow
      if ( events.size() == 0 ) throw e;
    }
    return std::make_shared<MMapBuffer>( std::move( mmapBuffer ), std::move( events ) );
  }

} // namespace

SharedMMappedFile InputHandler::connectToCurrentInput() {
  while ( m_curInput < m_input.size() ) {
    try {
      return std::make_shared<LHCb::MDF::MMappedFile>( m_input[m_curInput] );
    } catch ( std::string const& error ) {
      m_ioSvc->error() << error << endmsg;
      m_curInput++;
    }
  }
  throw LHCb::IIOSvc::EndOfInput();
}

LHCb::MDFHeader* InputHandler::readNextEventHeader() {
  // Check for end of file
  if ( m_curMappedFile->end() <= m_curPtr + sizeof( LHCb::MDFHeader ) ) {
    // Prepare next file for next call
    m_curInput++;
    m_curMappedFile = connectToCurrentInput();
    m_curPtr        = m_curMappedFile->data().data();
    // End of the file reached
    m_ioSvc->info() << "Over with input from " << m_input[m_curInput] << endmsg;
    // throw End of file exception
    throw EndOfFile();
  }
  // standard case, just return current position in file
  return reinterpret_cast<LHCb::MDFHeader*>( m_curPtr );
}

void InputHandler::skip( unsigned int numEvt ) {
  for ( unsigned int i = 0; i < numEvt; ++i ) { skip( *readNextEventHeader() ); }
}

namespace LHCb::MDF {

  /**
   * Implementation of IOSvc for reading from standard file via mmap
   */
  class IOSvcMM : public IOSvc {

  public:
    using IOSvc::IOSvc;
    /// Service initialization
    StatusCode initialize() override;

  private:
    /**
     * preloads data into the next buffer
     */
    void preloadNextBuffer( std::unique_lock<std::mutex>& guard ) override;

  private:
    /// Helper Object for handling inputs
    std::unique_ptr<InputHandler> m_inputHandler{nullptr};
  };
} // namespace LHCb::MDF

DECLARE_COMPONENT( LHCb::MDF::IOSvcMM )

StatusCode LHCb::MDF::IOSvcMM::initialize() {
  StatusCode sc = Service::initialize();
  if ( !sc.isSuccess() ) {
    error() << "Unable to initialize base class Service." << endmsg;
    return sc;
  }
  try {
    // check there are inputs
    if ( m_input.value().size() == 0 ) throw EndOfInput();
    // connect to first input
    m_inputHandler = std::make_unique<InputHandler>( this, m_input.value() );
    m_inputHandler->skip( m_nbSkippedEvents );
    // prefetch data in the current buffer
    m_curBuffer = prefetchEvents( std::ref( *m_inputHandler ), m_bufferNbEvents.value() );
    // and prefetch more data into the next buffer
    std::packaged_task<std::shared_ptr<LHCb::MDF::Buffer>( InputHandler&, unsigned int )> task( prefetchEvents );
    m_nextBuffer = task.get_future();
    task( std::ref( *m_inputHandler ), m_bufferNbEvents.value() );
  } catch ( EndOfInput& e ) {
    error() << "Empty input in IOSvc" << endmsg;
    return StatusCode::FAILURE;
  }
  return sc;
}

void LHCb::MDF::IOSvcMM::preloadNextBuffer( std::unique_lock<std::mutex>& guard ) {
  // use a task, and associate its future to next buffer
  std::packaged_task<std::shared_ptr<LHCb::MDF::Buffer>( InputHandler&, unsigned int )> task( prefetchEvents );
  m_nextBuffer = task.get_future();
  // now that next buffer is set, we can unlock to let other theads consume the current buffer
  // while we are preloading the next one
  guard.unlock();
  // and preload data into the next bufferby running the task
  task( std::ref( *this->m_inputHandler ), this->m_bufferNbEvents.value() );
}
