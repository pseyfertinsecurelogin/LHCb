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
// Include files

#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/GaudiException.h"

#include "DetCond/ICondDBReader.h"

#include <xercesc/util/XMLString.hpp>

#include "XmlTools/ValidInputSource.h"

#include "CoolKernel/IObject.h"
#include "CoolKernel/IRecord.h"
#include "CoolKernel/RecordException.h"

#include "CondDBCompression.h"

// local
#include "CondDBEntityResolver.h"

namespace {

// custom istream, which, when given an rvalue std::string,
// will 'steal' its buffer. This avoids a copy compared to
// std::istringstream... (which only accepts a string by const&)
template <typename char_type>
class basic_isstream : public std::basic_istream<char_type> {
  class isstreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type> >
  {
    std::basic_string<char_type> m_buffer;
  public:
    template <typename... Args>
    isstreambuf(Args&&... args) : m_buffer( std::forward<Args>(args)... ) {
      // there could be a write to this buffer through sputbackc, (i.e. istream::putback)
      // if one puts back a different character then originally there -- which would be
      // undefined behaviour, as writes to data() are undefined behaviour...
      // (think: cow string implementation where it doesn't realize there is a write,
      // but cow isn't C++11 conforming anyway)
      char_type* b = const_cast<char_type*>(m_buffer.data());
      this->setg( b, b, b+m_buffer.size() );
    }
  } m_sbuf;
public:
  // note: the buffer passed to the istream constructor hasn't had its constructor
  // called yet, but because the C++ standard requires istream to not use rdbuf in
  // it's constructor (or destructor) this is not a problem.
  template <typename ... Args>
  basic_isstream(Args&&... args) : std::istream(&m_sbuf), m_sbuf(std::forward<Args>(args)...) { }
};

using isstream = basic_isstream<char>;

}


#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define DEBUG_MSG ON_DEBUG debug()
#define ON_VERBOSE if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
#define VERBOSE_MSG ON_VERBOSE verbose()


//-----------------------------------------------------------------------------
// Implementation file for class : CondDBEntityResolverSvc
//
// 2005-10-18 : Marco Clemencic
//-----------------------------------------------------------------------------

// Factory implementation
DECLARE_COMPONENT( CondDBEntityResolver )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CondDBEntityResolver::CondDBEntityResolver( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent ):
  base_class(type, name, parent)
{
  declareProperty(m_detDataSvcName.name(), m_detDataSvcName);
  declareProperty(m_condDBReaderName.name(), m_condDBReaderName);
}

//=========================================================================
//  Initialize the service
//=========================================================================
StatusCode CondDBEntityResolver::initialize ( ) {
  StatusCode sc = AlgTool::initialize();
  if ( ! sc.isSuccess() ) return sc;

  DEBUG_MSG << "Initializing..." << endmsg;

  // Initialize the Xerces-C++ XML subsystem
  try {

    xercesc::XMLPlatformUtils::Initialize();

    DEBUG_MSG << "Successfully initialized." << endmsg;

  } catch(const xercesc::XMLException& toCatch) {
    char *message = xercesc::XMLString::transcode(toCatch.getMessage());
    fatal() << "Error during Xerces-C Initialization." << endmsg;
    fatal() << "  Exception message:" << message << endmsg;
    xercesc::XMLString::release(&message);
    sc = StatusCode::FAILURE;  // allow return value optimization
  }

  return sc;
}

//=========================================================================
//  Finalize the service
//=========================================================================
StatusCode CondDBEntityResolver::finalize ( ) {

  m_condDBReader.reset();
  m_detDataSvc.reset();

  // Finalize the Xerces-C++ XML subsystem
  xercesc::XMLPlatformUtils::Terminate();

  return AlgTool::finalize();
}

//=========================================================================
// Return the pointer to the CondDBReader (loading it if not yet done).
//=========================================================================
ICondDBReader *CondDBEntityResolver::condDBReader() const {
  if (!m_condDBReader) {
    m_condDBReader = service(m_condDBReaderName, true);
    if( !m_condDBReader) {
      throw GaudiException("Can't locate service " + m_condDBReaderName.value(),
                           name(), StatusCode::FAILURE);
    } else {
     DEBUG_MSG << "Successfully located service " << m_condDBReaderName.value() << endmsg;
    }
  }
  return m_condDBReader;
}
//=========================================================================
// Return the pointer to the detector data service (loading it if not yet done).
//=========================================================================
IDetDataSvc *CondDBEntityResolver::detDataSvc() {
  if (!m_detDataSvc) {
    m_detDataSvc = service(m_detDataSvcName, true);
    if( !m_detDataSvc ) {
      throw GaudiException("Can't locate service " + m_detDataSvcName.value(),
                           name(), StatusCode::FAILURE);
    } else {
      DEBUG_MSG << "Successfully located service " << m_detDataSvcName.value() << endmsg;
    }
  }
  return m_detDataSvc;
}
//=========================================================================
// fill validity limits and data (str) with the content retrieved from the url
//=========================================================================
boost::optional<std::string> CondDBEntityResolver::i_getData(boost::string_ref url, Gaudi::Time &since, Gaudi::Time &until ) {

  std::string path{"/"};
  boost::string_ref orig_url = url;

  // strip optional prefix "coddb:"
  if ( url.starts_with( "conddb:" ) ) url.remove_prefix(7);
  // strip leading '/'s
  url.remove_prefix(url.find_first_not_of('/'));


  // extract the channel id
  cool::ChannelId channel = 0;
  auto colon_pos = url.find( ':' );
  if ( colon_pos != url.npos ) { // It means that I have a ':' in the url
    std::istringstream oss( url.substr( colon_pos + 1 ).to_string() );
    oss >> channel;
    url.remove_suffix( url.length() - colon_pos );
    ON_DEBUG {
      debug() << "Found channel id in the URI" << endmsg;
      debug() << "path   = /" << url << endmsg;
      debug() << "ch. id = " << channel << endmsg;
    }
  }

  // Extract the COOL field name from the condition url
  // "conddb:/path/to/field@folder"
  boost::string_ref data_field_name = "data"; // default value
  auto at_pos = url.find( '@' );
  if ( at_pos != url.npos ) {
    auto start = url.substr(0, at_pos).rfind( '/' ) + 1; // it works also if there is no '/' because (npos + 1) == 0
    if ( start < at_pos ) { // item name is not null
      data_field_name = url.substr( start, at_pos - start );
    } // if I have "/@", I should use the default ("data")
    // always remove '@' from the url
    auto before = url.substr(0, start);
    auto after = url.substr(at_pos + 1);
    path.append( before.data(), before.length() ).append( after.data(), after.length() );
  } else {
    path.append( url.data(), url.length() );
  }

  Gaudi::Time now;
  if ( detDataSvc()->validEventTime() ) {
    now = detDataSvc()->eventTime();
  } else {
    error() << "event time undefined" << endmsg;
    return boost::none;
  }

  // outputs
  std::string descr;
  ICondDBReader::DataPtr data;

  StatusCode sc = condDBReader()->getObject(path, now, data, descr, since, until, channel);
  if ( ! sc.isSuccess() ) return boost::none;

  if ( ! data ) {
    error() << "Cannot find any data at " << orig_url << endmsg;
    return boost::none;
  }

  try {

    return CondDBCompression::decompress((*data)[data_field_name.to_string()].data<std::string>());

  } catch (cool::RecordSpecificationUnknownField &e) {
    error() << "I cannot find the data inside COOL object: " << e.what() << endmsg;
  }

  return boost::none;
}

//=========================================================================
//  Returns an input stream to read from the opened file.
//=========================================================================
CondDBEntityResolver::open_result_t CondDBEntityResolver::open(const std::string &url) {

  Gaudi::Time since, until;
  auto str = i_getData(url,since,until);
  if (!str){
    throw GaudiException("Cannot open URL " + url, name(), StatusCode::FAILURE);
  }
  return open_result_t(new isstream(std::move(*str)));
}
//=========================================================================
//  Returns the list of supported protocols.
//=========================================================================
const std::vector<std::string> &CondDBEntityResolver::protocols() const {
  /// Vector of supported protocols. (for IFileAccess)
  static const std::vector<std::string> s_protocols = { { "conddb" } };
  return s_protocols;
}
//=========================================================================
//  Create a Xerces-C input source based on the given systemId (publicId is ignored).
//=========================================================================
xercesc::InputSource *CondDBEntityResolver::resolveEntity(const XMLCh *const, const XMLCh *const systemId) {

  // copy Xerces string into std::string
  std::string systemIdString;
  char *cString = xercesc::XMLString::transcode(systemId);
  if (cString) {
    systemIdString = cString;
    xercesc::XMLString::release(&cString);
  }

  DEBUG_MSG << "systemId = \"" << systemIdString << "\"" << endmsg;

  if ( systemIdString.compare(0, 7, "conddb:") != 0 ) {
    // the string does not start with "conddb:", so I cannot handle it
    VERBOSE_MSG << "Not a conddb URL" << endmsg;
    // tell XercesC to use the default action
    return nullptr;
  }

  Gaudi::Time since, until;
  auto str = i_getData(systemIdString, since, until);

  if (!str) {
    // tell XercesC to use the default action
    return nullptr;
  }

  // _move_ the string into the inputSource
  ValidInputSource *inputSource = new ValidInputSource(std::move(*str),
                                                       systemId);
  inputSource->setSystemId(systemId);
  inputSource->setValidity(since, until);

  // Done!
  return inputSource;
}

void CondDBEntityResolver::defaultTags ( std::vector<LHCb::CondDBNameTagPair>& tags ) const {
  condDBReader()->defaultTags( tags );
}
//=============================================================================
