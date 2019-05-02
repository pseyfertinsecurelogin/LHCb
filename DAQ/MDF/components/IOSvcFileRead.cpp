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

#include "MDF/IOSvc.h"
#include "MDF/MDFHeader.h"
#include "MDF/RawDataConnection.h"

#include "GaudiUtils/IIODataManager.h"

#include <mutex>

namespace {

  struct free_deleter {
    template <typename T>
    void operator()( T* p ) const {
      std::free( const_cast<std::remove_const_t<T>*>( p ) );
    }
  };

  template <typename T>
  using unique_ptr_free = std::unique_ptr<T, free_deleter>;

  using ByteBuffer = LHCb::MDF::OwningBuffer<unique_ptr_free<std::byte>>;

  /**
   * helper class dealing with a set of input files and able to read/seek from them
   */
  class InputHandler {
  public:
    struct SeekError {};
    struct ReadError {};

    /// constructor
    /// connects to first input already and thus may throw EndOfInput
    InputHandler( LHCb::MDF::IOSvc* ioSvc, std::vector<std::string> const& input, SmartIF<Gaudi::IIODataManager> ioMgr )
        : m_input( input ), m_ioMgr( ioMgr ), m_ioSvc( ioSvc ) {
      connectToCurrentInput();
    }
    /// destructor
    ~InputHandler() { m_ioMgr.reset(); }

    /// returns current input's name
    std::string const& inputName() { return m_input[m_curInput]; }
    /**
     * skips next n events without reading them (only headers will be actually read)
     * @throws SeekError
     * @throws IIOSvc::EndOfInput
     */
    void skip( unsigned int );
    /**
     * wrapper around seek method of IOMgr, handling errors
     * @param offset how much to seek from current place
     * @throws SeekError
     */
    void seek( int offset ) {
      if ( m_ioMgr->seek( &m_curConnection, offset, SEEK_CUR ) == -1 ) { throw SeekError(); }
    }
    /**
     * wrapper around read method of IOMgr, handling errors
     * @param buffer the buffer where to put the data
     * @param size number of bytes to read
     * @throws ReadError
     */
    /// wrapper around read method of IOMgr, handling errors and throwing ReadError
    void read( LHCb::span<std::byte> buffer ) {
      auto sc = m_ioMgr->read( &m_curConnection, buffer.data(), buffer.size() );
      if ( sc.isFailure() ) { throw ReadError(); }
    }
    /**
     * reads the next Event header into given buffer
     * buffer must be large enough or the behavior is undefined
     * @throws IIOSvc::EndOfInput
     */
    void readNextEventHeader( LHCb::MDFHeader& );

  private:
    /**
     * connects to current input
     * @throws IIOSvc::EndOfInput
     */
    void connectToCurrentInput();

  private:
    /// vector of input files
    std::vector<std::string> const& m_input;
    /// currently used input as an index in m_input
    unsigned int m_curInput{0};
    /// connection to current input, only valid if connected
    LHCb::RawDataConnection m_curConnection{0, ""};
    /// Reference to file manager service
    SmartIF<Gaudi::IIODataManager> m_ioMgr;
    /// pointer to IOSvc
    LHCb::MDF::IOSvc* m_ioSvc;
  };

  /**
   * prefetches a number of events from input, allocating a Buffer object to store them and returning it
   * It will try to fill the buffer unless input is empty
   * @param bufferSize the size of the buffer to allocate
   */
  std::shared_ptr<LHCb::MDF::Buffer> prefetchEvents( InputHandler& input, unsigned int nbEvents ) {
    // allocate a new buffer according to requested size, taking 50K per event
    // 50K is low, this is to ensure that in most cases we will not reallocate the event vector
    unsigned int bufferSize = nbEvents * 50000;
    auto         buffer     = unique_ptr_free<std::byte>{reinterpret_cast<std::byte*>( std::malloc( bufferSize ) )};
    if ( nullptr == buffer ) { throw( "Unable to allocate memory for new buffer" ); }
    // create associated events vector and reserve space
    std::vector<LHCb::MDF::MDFEvent> events;
    events.reserve( nbEvents );
    // Fill buffer with banks while there is enough space and create associated events
    auto curBufPtr  = buffer.get();
    auto headerSize = sizeof( LHCb::MDFHeader );
    while ( curBufPtr + headerSize < buffer.get() + bufferSize ) {
      LHCb::MDFHeader* header = reinterpret_cast<LHCb::MDFHeader*>( curBufPtr );
      try {
        input.readNextEventHeader( *header );
      } catch ( LHCb::IIOSvc::EndOfInput& e ) {
        // we've reached the end of the input
        // if we have data, just break, else rethrow
        if ( curBufPtr == buffer.get() ) throw e;
        break;
      }
      if ( curBufPtr + header->recordSize() >= buffer.get() + bufferSize ) {
        // buffer is full, let's stop here and rewind the input before the header
        input.seek( -headerSize );
        break;
      }
      // enough space for rawbanks of the next event in buffer. Let's first copy the banks to the buffer
      input.read( {curBufPtr + headerSize, (long)( header->recordSize() - headerSize )} );
      // now let's build the event and copy the raw banks
      events.emplace_back( LHCb::span<std::byte>{(std::byte*)header->data(), header->size()} );
      curBufPtr += header->recordSize();
    }
    return std::make_shared<ByteBuffer>( std::move( buffer ), std::move( events ) );
  }

} // namespace

void InputHandler::connectToCurrentInput() {
  while ( m_curInput < m_input.size() ) {
    m_curConnection = LHCb::RawDataConnection( m_ioSvc, m_input[m_curInput] );
    auto sc         = m_ioMgr->connectRead( false, &m_curConnection );
    if ( sc.isSuccess() ) { return; }
    m_ioSvc->error() << "could not connect to input " << m_input[m_curInput] << endmsg;
    m_curInput++;
  }
  throw LHCb::IIOSvc::EndOfInput();
}

void InputHandler::readNextEventHeader( LHCb::MDFHeader& header ) {
  StatusCode sc = m_ioMgr->read( &m_curConnection, (std::byte*)&header, sizeof( LHCb::MDFHeader ) );
  while ( !sc.isSuccess() ) {
    // No space left ? Or input error ? Anyway close current input
    m_ioSvc->info() << "Over with input from " << m_input[m_curInput] << " : " << sc << endmsg;
    m_ioMgr->disconnect( &m_curConnection ).ignore();
    // and connect to next one
    m_curInput++;
    connectToCurrentInput();
    if ( m_curInput >= m_input.size() ) { throw LHCb::IIOSvc::EndOfInput(); }
    // try again to read header
    sc = m_ioMgr->read( &m_curConnection, (std::byte*)&header, sizeof( LHCb::MDFHeader ) );
  }
}

void InputHandler::skip( unsigned int numEvt ) {
  LHCb::MDFHeader header;
  for ( unsigned int i = 0; i < numEvt; ++i ) {
    readNextEventHeader( header );
    auto offset = m_ioMgr->seek( &m_curConnection, header.recordSize() - 3 * sizeof( int ), SEEK_CUR );
    if ( offset == -1 ) throw SeekError();
  }
}

namespace LHCb::MDF {

  /**
   * Implementation of IOSvc for reading from standard files
   */
  class IOSvcFileRead : public IOSvc {

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

    Gaudi::Property<std::string> m_ioMgrName{this, "DataManager", "Gaudi::IODataManager/IODataManager",
                                             "Name of the file manager service"};
  };

} // namespace LHCb::MDF

DECLARE_COMPONENT( LHCb::MDF::IOSvcFileRead )

StatusCode LHCb::MDF::IOSvcFileRead::initialize() {
  StatusCode sc = IOSvc::initialize();
  if ( !sc.isSuccess() ) {
    error() << "Unable to initialize base class IOSvc." << endmsg;
    return sc;
  }
  // Retrieve conversion service handling event iteration
  auto ioMgr = Service::service( m_ioMgrName ).as<Gaudi::IIODataManager>();
  if ( !ioMgr ) {
    error() << "Unable to localize interface IID_IIODataManager from service:" << m_ioMgrName << endmsg;
    return StatusCode::FAILURE;
  }
  try {
    // check there are inputs
    if ( m_input.value().size() == 0 ) throw LHCb::IIOSvc::EndOfInput();
    // connect to first input
    m_inputHandler = std::make_unique<InputHandler>( this, m_input.value(), ioMgr );
    m_inputHandler->skip( m_nbSkippedEvents );
    // prefetch data in the current buffer
    m_curBuffer = prefetchEvents( std::ref( *m_inputHandler ), m_bufferNbEvents.value() );
    // and prefetch more data into the next buffer
    std::packaged_task<std::shared_ptr<LHCb::MDF::Buffer>( InputHandler&, unsigned int )> task( prefetchEvents );
    m_nextBuffer = task.get_future();
    task( std::ref( *m_inputHandler ), m_bufferNbEvents.value() );
  } catch ( LHCb::IIOSvc::EndOfInput& e ) {
    error() << "Empty input in IOSvcFileRead" << endmsg;
    return StatusCode::FAILURE;
  }
  return sc;
}

void LHCb::MDF::IOSvcFileRead::preloadNextBuffer( std::unique_lock<std::mutex>& guard ) {
  // use a task, and associate its future to next buffer
  std::packaged_task<std::shared_ptr<LHCb::MDF::Buffer>( InputHandler&, unsigned int )> task( prefetchEvents );
  m_nextBuffer = task.get_future();
  // now that next buffer is set, we can unlock to let other theads consume the current buffer
  // while we are preloading the next one
  guard.unlock();
  // and preload data into the next bufferby running the task
  task( std::ref( *this->m_inputHandler ), this->m_bufferNbEvents.value() );
}
