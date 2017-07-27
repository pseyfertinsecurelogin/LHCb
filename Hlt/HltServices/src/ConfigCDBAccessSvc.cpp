#include "ConfigCDBAccessSvc.h"
#include "cdb.h"

#include <map>
#include <ctime>
#include <limits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "boost/iostreams/copy.hpp"
#include "boost/iostreams/filter/zlib.hpp"
#ifndef _WIN32
#include "boost/iostreams/filter/bzip2.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/device/back_inserter.hpp"
#include "boost/iostreams/device/array.hpp"
#include "boost/iostreams/stream.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include "GaudiKernel/System.h"
#include "GaudiKernel/IIncidentSvc.h"

namespace io = boost::iostreams;
namespace fs = boost::filesystem;

namespace
{

std::string operator+(boost::string_ref lhs, boost::string_ref rhs)
{
    std::string s; s.reserve(lhs.size()+rhs.size());
    return s.append(lhs.data(),lhs.size()).append(rhs.data(),rhs.size());
}

std::string toString( boost::string_ref sr )
{
    return { sr.data(), sr.size() };
}

struct DefaultFilenameSelector
{
    bool operator()( boost::string_ref /*fname*/ ) const
    {
        return true;
    }
};

struct PrefixFilenameSelector
{
    PrefixFilenameSelector( std::string  _prefix ) : prefix(std::move( _prefix ))
    {
    }
    bool operator()( boost::string_ref fname ) const
    {
        return fname.starts_with(prefix);
    }
    std::string prefix;
};

uint8_t read8( std::istream& s )
{
    union { unsigned int ui; int i; } ;
    i = s.get(); // stream will typically return an 'int', as it is a 'char' stream
    return ui;
}

uint16_t read16( std::istream& s )
{
    auto r1 = uint16_t( read8( s ) );
    auto r2 = uint16_t( read8( s ) ) << 8;
    return r1 | r2 ;
}

uint32_t read32( std::istream& s )
{
    auto r1 = uint32_t( read8( s ) );
    auto r2 = uint32_t( read8( s ) ) <<  8;
    auto r3 = uint32_t( read8( s ) ) << 16;
    auto r4 = uint32_t( read8( s ) ) << 24;
    return r1 | r2 | r3 | r4;
}

uid_t read_uid( std::istream& s )
{
    return read32( s );
}

std::time_t read_time( std::istream& s )
{
    return read32( s );
}

int compress( std::string& str )
{
    // compress and check if worthwhile...
    std::string compressed;
    compressed.reserve( str.size() );
    io::filtering_streambuf<io::output> filter;
    io::zlib_params params;
    params.noheader = true;
    filter.push( io::zlib_compressor( params ) );
    filter.push( io::back_inserter( compressed ) );
    io::copy( boost::make_iterator_range( str ), filter, 8192 );
    bool ok = compressed.size() < str.size(); // yes, it's better!
    if ( ok ) str = std::move( compressed );
    return ok ? 3 : 0;
}

std::vector<unsigned char> make_cdb_record( std::string str, uid_t uid, std::time_t t )
{
    auto flags = compress( str );
    std::vector<unsigned char> buffer; buffer.reserve( 12 + str.size() );
    buffer.emplace_back( 0u );     // version number
    buffer.emplace_back( flags ); // compression
    buffer.emplace_back( 0u );     // reserved;
    buffer.emplace_back( 0u );     // reserved;
    assert( sizeof( uid_t ) == 4 );
    buffer.emplace_back( uid & 0xff );
    buffer.emplace_back( ( uid >> 8 ) & 0xff );
    buffer.emplace_back( ( uid >> 16 ) & 0xff );
    buffer.emplace_back( ( uid >> 24 ) & 0xff );
    buffer.emplace_back( t & 0xff );
    buffer.emplace_back( ( t >> 8 ) & 0xff );
    buffer.emplace_back( ( t >> 16 ) & 0xff );
    buffer.emplace_back( ( t >> 24 ) & 0xff );
    if ( buffer.size() != 12 ) std::cerr << "CDB: ERROR unexpected header size" << std::endl;
    std::copy_n( begin( str ), str.size(), back_inserter(buffer) );
    if ( buffer.size() != 12 + str.size() ) std::cerr << "CDB: ERROR unexpected record size" << std::endl;
    return buffer;
}

class CloseListener : public implements<IIncidentListener> {
public:
   CloseListener(std::string incident, std::unique_ptr<ConfigCDBAccessSvc_details::CDB> &file)
      : m_incident{std::move(incident)}, m_file(file) {
         addRef(); // Initial count set to 1
      }

   /// Inform that a new incident has occurred
   void handle(const Incident& i) override {
      if (i.type() == m_incident) m_file.reset();
   }

private:
   /// incident to handle
   const std::string m_incident;
   /// file to reset (close)
   std::unique_ptr<ConfigCDBAccessSvc_details::CDB> &m_file;
};

}

namespace ConfigCDBAccessSvc_details
{

class CDB
{
  public:
    CDB( const std::string& name, std::ios::openmode mode = std::ios::in ) : m_fname( name )
    {
        if ( mode & std::ios::out ) {
            m_oname = fs::unique_path( m_fname.string() + "-%%%%-%%%%-%%%%-%%%%" );
        }

        cdb_fileno( &m_ocdb ) = -1;;
        // if open 'readwrite' we construct a fresh copy of the input
        // database, then extend it, and on 'close' replace the original one
        // with the new one.

        // Note that while appending, we need to keep a 'shadow' copy to satisfy
        // reads to the newly written items...

        int fd = open( m_fname.c_str(), O_RDONLY );
        if ( fd < 0 ) {
            if (!writing() ) {
                m_error = true;
                throw std::runtime_error( "Error opening file " + m_fname.string() + ": " + strerror(errno) );
            }
            cdb_fileno(&m_icdb)=-1;
        } else {
            if ( cdb_init( &m_icdb, fd ) < 0 ) {
                throw std::runtime_error( "Could not inititalize CDB for " + m_fname.string() + ": " + strerror(errno) );
            }
        }

        if ( writing() ) {
            // From http://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html:
            //
            // O_CREAT and O_EXCL are set, open() will fail if the file exists. The
            // check for
            // the existence of the file and the creation of the file if it does not
            // exist will
            // be atomic with respect to other processes executing open() naming the
            // same
            // filename in the same directory with O_EXCL and O_CREAT set. If O_CREAT
            // is not set,
            // the effect is undefined.

            int ofd = open( m_oname.c_str(), O_WRONLY | O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
            if ( ofd < 0 ) {
                m_error = true;
                throw std::runtime_error( "Failed to create " + m_oname.string() + " exclusive, write-only: " + strerror(errno) );
            }
            if ( cdb_make_start( &m_ocdb, ofd )!= 0 ) {
                m_error = true;
                throw std::runtime_error( "Failed to initialize CDB for writing" );
            }

            // only copy if there is a valid input .cdb file!
            if ( fd >= 0 && ofd >= 0 ) {
                unsigned cpos;
                cdb_seqinit( &cpos, &m_icdb );
                while ( cdb_seqnext( &cpos, &m_icdb ) > 0 ) {
                    if ( cdb_make_add( &m_ocdb,
                                       static_cast<const unsigned char*>( cdb_getkey( &m_icdb ) ),
                                       cdb_keylen( &m_icdb ),
                                       static_cast<const unsigned char*>( cdb_getdata( &m_icdb ) ),
                                       cdb_datalen( &m_icdb ) ) != 0 ) {
                        // handle error...
                        m_error = true;
                        throw std::runtime_error(
                        "Failed to put key " + std::string( static_cast<const char*>(
                                                    cdb_getkey( &m_icdb ) ), cdb_keylen( &m_icdb ) )
                                               + " : " + strerror( errno ) );
                    }
                }
            }
        }
    }

    ~CDB()
    {
        if (cdb_fileno(&m_icdb)>=0) {
            close( cdb_fileno( &m_icdb ) );
            cdb_free( &m_icdb );
        }
        if ( writing() ) cleanup_ocdb(m_error);
    }

    bool writing() const
    {
        return !m_oname.empty();
    };

    template <typename T>
    bool readObject( T& t, boost::string_ref key )
    {
        // first check input database... (if it is available!)
        if ( cdb_fileno(&m_icdb) != -1 && cdb_find( &m_icdb, key.data(), key.size() ) > 0 ) {
            io::stream<io::array_source> value(
                static_cast<const char*>( cdb_getdata( &m_icdb ) ),
                cdb_datalen( &m_icdb ) );
            // 12 bytes of header information...
            {
                unsigned int version [[gnu::unused]] = read8( value );  // C++17: replace by [[maybe_unused]]; note: both clang and gcc understand [[gnu::unused]]
                assert( version == 0 );
            }
            unsigned int flags = read8( value );
            assert( flags < 4 );
            {
                unsigned int reserved [[gnu::unused]] = read16( value );
                assert( reserved == 0 );
                auto uid [[gnu::unused]] = read_uid( value );
                auto tm [[gnu::unused]] = read_time( value );
            }

            assert( value.tellg() == 12 );
            io::filtering_istream s;
            switch ( flags & 0x3 ) {
            case 0:
                break; // do nothing...
            case 2:
                s.push( io::bzip2_decompressor() );
                break;
            case 3: {
                io::zlib_params params;
                params.noheader = true;
                s.push( io::zlib_decompressor( params ) );
            } break;
            default:
                std::cerr << "CDB: unknown compression flag" << std::endl;
                return 0;
            }
            s.push( value );
            s >> t;
            assert( value.tellg() == cdb_datalen( &m_icdb ) );
            return true;
        }
        // not in input, check write cache?
        if ( writing() ) {
            auto i = m_write_cache.find( key );
            if ( i != m_write_cache.end() ) {
                std::stringstream s( i->second );
                s >> t;
                return true;
            }
        }
        return false;
    }

    template <typename SELECTOR>
    std::vector<std::string> files( const SELECTOR& selector )
    {
        std::vector<std::string> keys;
        if ( cdb_fileno( &m_icdb ) >= 0 ) {
            unsigned cpos;
            cdb_seqinit( &cpos, &m_icdb );
            while ( cdb_seqnext( &cpos, &m_icdb ) > 0 ) {
                boost::string_ref key( static_cast<const char*>( cdb_getkey( &m_icdb ) ),
                                       cdb_keylen( &m_icdb ) );
                if ( selector( key ) ) keys.emplace_back( key.data(), key.size() );
            }
        }
        if ( writing() ) { // then also check write cache...
            for ( auto& i : m_write_cache ) {
                if ( selector( i.first ) ) keys.push_back( i.first );
            }
        }
        return keys;
    }

    uid_t getUid() const
    {
#ifndef _WIN32
        if ( UNLIKELY(!m_myUid) ) m_myUid = getuid();
#endif
        return m_myUid;
    }



    bool append( boost::string_ref key, std::stringstream& is )
    {
        if ( !writing() || m_error ) return false;

        // first, look in input database
        std::string rd;
        if ( readObject( rd, key ) ) {
            bool ok = rd == is.str();
            if ( !ok ) {
                std::cerr << "append error: entry present in input , but not equal!"
                          << std::endl;
            }
            return ok;
        }
        // if not there, look in write cache
        auto i = m_write_cache.find( key );
        if ( i != m_write_cache.end() ) {
            bool ok = i->second == is.str();
            if ( !ok ) {
                std::cerr << "append error: entry present in output , but not equal!"
                          << std::endl;
            }
            return ok;
        }

        // aha, this is an as yet unknown key... insert it!
        m_write_cache.emplace( toString(key), is.str() );

        auto record = make_cdb_record( is.str(), getUid(), std::time(nullptr) );
        if ( cdb_make_add( &m_ocdb,
                           reinterpret_cast<const unsigned char*>( key.data() ),
                           key.size(), record.data(), record.size() ) != 0 ) {
            // handle error...
            m_error = cleanup_ocdb(true);
            throw std::runtime_error( std::string{"failure to put key "} +
                                      key + strerror(errno) );
        }
        return true;
    }
    bool operator!() const
    {
        return m_error;
    }

    std::string name() const { return m_fname.string(); }
  private:
    bool cleanup_ocdb(bool error) {
        auto fd = cdb_fileno( &m_ocdb );
        if (fd<0) return false;
        if (!error) {
            error = ( cdb_make_finish( &m_ocdb ) < 0 );
         } else {
            cdb_make_free(&m_ocdb);
         }
        close( fd );
        if ( !error ) {
            fs::rename( m_oname, m_fname );
        } else {
            std::cerr << "encountered an error; leaving original " << m_fname
                 << " untouched, removing temporary " << m_oname << std::endl;
            fs::remove( m_oname );
        }
        cdb_fileno( &m_ocdb ) = -1;
        return error;
    }

    struct cdb      m_icdb;
    struct cdb_make m_ocdb;
    fs::path        m_fname;
    fs::path        m_oname;
    std::map<std::string, std::string,std::less<>> m_write_cache; // write cache..
    mutable uid_t m_myUid = 0;
    bool m_error = false;
};
}
using namespace ConfigCDBAccessSvc_details;

// Factory implementation
DECLARE_COMPONENT( ConfigCDBAccessSvc )

//=============================================================================
// Initialization
//=============================================================================
StatusCode ConfigCDBAccessSvc::initialize()
{
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Initialize" << endmsg;
    StatusCode status = Service::initialize();
    if ( !status.isSuccess() ) return status;
    status = setProperties();

    // If requested, listen to an incident, such that the file we hold will be
    // closed when it it fired. In that case, we should be handled first/always,
    // so give maximum priority.
    if (status.isSuccess() && !m_incident.empty()) {
       m_initListener= std::make_unique<CloseListener>(m_incident, m_file);
       auto incSvc = service<IIncidentSvc>("IncidentSvc");
       incSvc->addListener(m_initListener.get(), m_incident, std::numeric_limits<long>::max());
    }
    return status;
}

ConfigCDBAccessSvc_details::CDB* ConfigCDBAccessSvc::file() const
{
    if ( UNLIKELY(!m_file) ) {
      std::unique_lock<std::mutex> lock(m_file_mtx);
      if (LIKELY(!m_file)){
        if ( m_mode != "ReadOnly" && m_mode != "ReadWrite" &&
             m_mode != "Truncate" ) {
            error() << "invalid mode: " << m_mode.value() << endmsg;
            return nullptr;
        }
  // todo: use Parse and toStream to make mode instead of m_mode a property...
        std::ios::openmode mode = ( m_mode == "ReadWrite"
                                 ? ( std::ios::in | std::ios::out | std::ios::ate )
                                 : ( m_mode == "Truncate" )
                                       ? ( std::ios::in | std::ios::out | std::ios::trunc )
                                       : std::ios::in );
        std::string name = m_name.value();
        if ( name.empty() ) {
            std::string def( System::getEnv( "HLTTCKROOT" ) );
            if ( def.empty() ) {
               throw GaudiException("Environment variable HLTTCKROOT not specified and no explicit "
                                    "filename given; cannot obtain location of config.cdb.",
                                    this->name(), StatusCode::FAILURE);
            }
            name = def + "/config.cdb";
        }
        info() << " opening " << name << " in mode " << m_mode.value() << endmsg;
        try {
            m_file = std::make_unique<CDB>( name, mode );
        } catch (const std::runtime_error& err) {
            error() << err.what() << endmsg;
            m_file.reset( );
        }
      }
    }
    return m_file.get();
}

//=============================================================================
// Stop
//=============================================================================
StatusCode ConfigCDBAccessSvc::stop()
{
    m_file.reset( ); // close file if still open
    return Service::stop();
}

//=============================================================================
// Finalization
//=============================================================================
StatusCode ConfigCDBAccessSvc::finalize()
{
    m_file.reset( ); // close file if still open
    return Service::finalize();
}

std::string ConfigCDBAccessSvc::propertyConfigPath(
    const PropertyConfig::digest_type& digest ) const
{
    return std::string( "PC/" ) += digest.str();
}

std::string ConfigCDBAccessSvc::configTreeNodePath(
    const ConfigTreeNode::digest_type& digest ) const
{
    return std::string( "TN/" ) += digest.str();
}

std::string ConfigCDBAccessSvc::configTreeNodeAliasPath(
    const ConfigTreeNodeAlias::alias_type& alias ) const
{
    return std::string( "AL/" ) += alias.str();
}

template <typename T>
boost::optional<T> ConfigCDBAccessSvc::read( const std::string& path ) const
{
    if ( msgLevel( MSG::DEBUG ) ) debug() << "trying to read " << path << endmsg;
    if ( !file() ) {
        if (msgLevel(MSG::DEBUG)) debug() << "no open file!" << endmsg;
        return {};
    }
    T c;
    if ( !file()->readObject( c, path ) ) {
        if ( msgLevel( MSG::DEBUG ) )
            debug() << "file " << path << " not found in container " << file()->name()
                    << endmsg;
        return {};
    }
    return c;
}

template <typename T>
bool ConfigCDBAccessSvc::write( const std::string& path, const T& object ) const
{
    if (!file()) {
        debug() << "no open file!" << endmsg;
        return false;
    }
    auto current = read<T>( path );
    if ( current ) {
        if ( object == current.get() ) return true;
        error() << " object @ " << path
                << "  already exists, but contents are different..." << endmsg;
        return false;
    }
    if ( m_mode.value() == "ReadOnly" ) {
        error() << "attempted write, but file has been opened ReadOnly" << endmsg;
        return false;
    }
    std::stringstream s;
    s << object;
    try {
        return file() && file()->append( path, s );
    } catch (std::runtime_error& err) {
        error() << "failure during write: " << err.what() << endmsg;
        return false;
    }
}

boost::optional<PropertyConfig>
ConfigCDBAccessSvc::readPropertyConfig( const PropertyConfig::digest_type& ref )
{
    return this->read<PropertyConfig>( propertyConfigPath( ref ) );
}

boost::optional<ConfigTreeNode>
ConfigCDBAccessSvc::readConfigTreeNode( const ConfigTreeNode::digest_type& ref )
{
    return this->read<ConfigTreeNode>( configTreeNodePath( ref ) );
}

boost::optional<ConfigTreeNode> ConfigCDBAccessSvc::readConfigTreeNodeAlias(
    const ConfigTreeNodeAlias::alias_type& alias )
{
    std::string fnam = configTreeNodeAliasPath( alias );
    boost::optional<std::string> sref = this->read<std::string>( fnam );
    if ( !sref ) return boost::none;
    ConfigTreeNode::digest_type ref =
        ConfigTreeNode::digest_type::createFromStringRep( *sref );
    if ( !ref.valid() ) {
        error() << "content of " << fnam << " not a valid ref" << endmsg;
        return boost::none;
    }
    return readConfigTreeNode( ref );
}

std::vector<ConfigTreeNodeAlias> ConfigCDBAccessSvc::configTreeNodeAliases(
    const ConfigTreeNodeAlias::alias_type& alias )
{
    std::vector<ConfigTreeNodeAlias> x;

    std::string basename( "AL" );
    if ( !file() ) return x;
    for ( const auto& i : file()->files( PrefixFilenameSelector( basename + "/" + alias.major() ) )) {
        // TODO: this can be more efficient...
        if ( msgLevel( MSG::DEBUG ) )
            debug() << " configTreeNodeAliases: adding file " << i << endmsg;
        std::string ref;
        file()->readObject( ref, i );
        std::stringstream str;
        str << "Ref: " << ref << '\n' << "Alias: " << i.substr( basename.size() + 1 )
            << '\n';
        ConfigTreeNodeAlias a;
        str >> a;
        if ( msgLevel( MSG::DEBUG ) )
            debug() << " configTreeNodeAliases: content:" << a << endmsg;
        x.push_back( a );
    }
    return x;
}

PropertyConfig::digest_type
ConfigCDBAccessSvc::writePropertyConfig( const PropertyConfig& config )
{
    PropertyConfig::digest_type digest = config.digest();
    return this->write( propertyConfigPath( digest ), config )
               ? digest
               : PropertyConfig::digest_type::createInvalid();
}

ConfigTreeNode::digest_type
ConfigCDBAccessSvc::writeConfigTreeNode( const ConfigTreeNode& config )
{
    ConfigTreeNode::digest_type digest = config.digest();
    return this->write( configTreeNodePath( digest ), config )
               ? digest
               : ConfigTreeNode::digest_type::createInvalid();
}

ConfigTreeNodeAlias::alias_type
ConfigCDBAccessSvc::writeConfigTreeNodeAlias( const ConfigTreeNodeAlias& alias )
{
    // verify that we're pointing at something existing
    if ( !readConfigTreeNode( alias.ref() ) ) {
        error() << " Alias points at non-existing entry " << alias.ref()
                << "... refusing to create." << endmsg;
        return ConfigTreeNodeAlias::alias_type();
    }
    // now write alias...
    fs::path fnam = configTreeNodeAliasPath( alias.alias() );
    boost::optional<std::string> x = read<std::string>( fnam.string() );
    if ( !x ) {
        std::stringstream s;
        s << alias.ref();
        if ( !file() ) {
            error() << " container file not found during attempted write of "
                    << fnam.string() << endmsg;
            return ConfigTreeNodeAlias::alias_type();
        }
        if ( file()->append( fnam.string(), s ) ) {
            info() << " created " << fnam.string() << endmsg;
            return alias.alias();
        } else {
            error() << " failed to write " << fnam.string() << endmsg;
            return ConfigTreeNodeAlias::alias_type();
        }
    } else {
        //@TODO: decide policy: in which cases do we allow overwrites of existing
        //labels?
        // (eg. TCK aliases: no!, tags: maybe... , toplevel: impossible by
        // construction )
        // that policy should be common to all implementations, so move to a mix-in
        // class,
        // or into ConfigTreeNodeAlias itself
        if ( ConfigTreeNodeAlias::digest_type::createFromStringRep( *x ) ==
             alias.ref() )
            return alias.alias();
        error() << " Alias already exists, but contents differ... refusing to change"
                << endmsg;
        return ConfigTreeNodeAlias::alias_type();
    }
}
