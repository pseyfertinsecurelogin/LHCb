#include "ConfigCDBAccessSvc.h"
#include "cdb.h"

#include <unordered_map>
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
#include "boost/algorithm/string/predicate.hpp"

#include "GaudiKernel/System.h"
#include "GaudiKernel/IIncidentSvc.h"

#include <regex>

namespace io = boost::iostreams;
namespace fs = boost::filesystem;

namespace {

ConfigTreeNodeAlias make_ConfigTreeNodeAlias( std::string_view ref, std::string_view alias ) {
    std::stringstream str;
    str << "Ref: " << ref << "\nAlias: " << alias << '\n';
    ConfigTreeNodeAlias a;
    str >> a;
    return a;
}

std::string propertyConfigPath( const PropertyConfig::digest_type& digest )
{ return "PC/" + digest.str(); }

std::string configTreeNodePath( const ConfigTreeNode::digest_type& digest )
{ return "TN/" + digest.str(); }

std::string configTreeNodeAliasPath( const ConfigTreeNodeAlias::alias_type& alias )
{ return "AL/" + alias.str(); }

struct DefaultFilenameSelector
{
    bool operator()( std::string_view ) const { return true; }
};

struct PrefixFilenameSelector
{
    PrefixFilenameSelector( std::string  _prefix ) : prefix(std::move( _prefix )) { }
    bool operator()( std::string_view fname ) const { return boost::algorithm::starts_with(fname,prefix); }
    std::string prefix;
};

uint8_t read8( std::istream& s )
{
    static_assert(sizeof(std::istream::char_type)==sizeof(uint8_t),"");
    std::istream::char_type c;  s.get(c);
    // if ( UNLIKELY(!s) ) throw std::runtime_error("error reading a character from stream");
    uint8_t ui; std::memcpy( &ui, &c, sizeof(uint8_t) );
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

struct cdb_record {
    const void *key,*data;
    size_t keylen, datalen;
    std::string_view string_key() const { return { static_cast<const char*>(key), keylen }; }
};

class icdb {
    struct cdb  m_db;
    std::string m_name;
public:
    explicit icdb( std::string fname ) : m_name(std::move(fname)) {
        auto fd = ::open( m_name.c_str(), O_RDONLY );
        if (fd < 0 || cdb_init( &m_db, fd ) < 0) cdb_fileno(&m_db)=-1;
    }
    icdb(const icdb&) = delete;
    icdb& operator=( const icdb& ) = delete;
    icdb(icdb&&) = delete;
    icdb& operator=( icdb&& ) = delete;
    ~icdb() {
        auto fd = cdb_fileno(&m_db);
        if (fd>=0) {
            close(fd);
            cdb_free(&m_db);
        }
    }

    explicit operator bool() const { return cdb_fileno(&m_db)>=0; }

    const std::string& name() const { return m_name; }

    class iterator {
        struct cdb* m_db = nullptr;
        unsigned m_cpos = 0;
        bool atEnd() const { return !m_db; }
    public:
        iterator(struct cdb *parent, unsigned cpos ) : m_db{parent}, m_cpos(cpos) {}
        explicit iterator(struct cdb *parent = nullptr) : m_db{parent} {
            if ( m_db ) cdb_seqinit(&m_cpos, m_db);
            ++*this;
        }
        iterator& operator++() {
            if ( m_db && cdb_seqnext(&m_cpos, m_db) <= 0 ) m_db = nullptr ;
            return *this;
        }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return   lhs.atEnd() != rhs.atEnd()
                || ( lhs.m_cpos != rhs.m_cpos && ( !lhs.atEnd() || !rhs.atEnd() ) );
        }
        cdb_record operator*() const { // Not quite canonical -- ideally should be value_type&...
            return { cdb_getkey(m_db), cdb_getdata(m_db),
                     cdb_keylen(m_db), cdb_datalen(m_db) };
        }
    };

    auto begin() { return iterator{ &m_db }; }
    auto end() const { return iterator{ }; }

    auto find(std::string_view key) {
        // Hrmpf: use inside knowledge of the (very stable) layout of the cdb structure...
        return cdb_find(&m_db, key.data(), key.size())>0 ?
                    iterator{ &m_db, cdb_datapos(&m_db) + cdb_datalen(&m_db) } :
                    end() ;
    }
};

class ocdb {
    struct cdb_make  m_db;
    std::string m_name;
    bool m_error = false;
public:
    ocdb( std::string fname ) : m_name(std::move(fname)) {
        // From http://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html:
        //
        // O_CREAT and O_EXCL are set, open() will fail if the file exists. The check for
        // the existence of the file and the creation of the file if it does not exist will
        // be atomic with respect to other processes executing open() naming the same
        // filename in the same directory with O_EXCL and O_CREAT set. If O_CREAT
        // is not set, the effect is undefined.
        auto fd = open( m_name.c_str(), O_WRONLY | O_CREAT | O_EXCL,
                                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
        if (fd < 0 ) {
            cdb_fileno(&m_db) = -1;
            throw std::runtime_error( "Failed to create " + m_name + " exclusive, write-only: " + strerror(errno) );
        }
        if ( cdb_make_start( &m_db, fd ) != 0 ) {
            ::close(fd);
            cdb_fileno(&m_db)=-1;
            fs::remove( m_name );
            throw std::runtime_error( "Failed to initialize CDB file " + m_name + " for writing" );
        }
    }
    ocdb( const ocdb& ) = delete;
    ocdb& operator=( const ocdb& ) = delete;
    ocdb( ocdb&& ) = delete;
    ocdb& operator=( ocdb&& ) = delete;
    ~ocdb() { if (*this) { cdb_make_free(&m_db); ::close(cdb_fileno(&m_db)); fs::remove(m_name); } }

    const std::string& name() const { return m_name; }

    void insert(std::string_view key, const std::vector<unsigned char>& data) {
        insert( cdb_record{ key.data(), data.data(), key.size(), data.size() });
    }
    void insert(cdb_record record) {
         if ( !*this || m_error
                     || cdb_make_add( &m_db, static_cast<const unsigned char*>(record.key),  record.keylen,
                                             static_cast<const unsigned char*>(record.data), record.datalen ) != 0 ) {
            m_error = true;
            throw std::runtime_error( "Failed to put key " + std::string{record.string_key()} );
         }
    }

    bool close() {
        if (!*this) return false;
        bool ok =  (cdb_make_finish(&m_db) == 0) && !m_error;
        ::close(cdb_fileno(&m_db));
        cdb_fileno(&m_db)=-1;
        if (!ok) fs::remove(m_name);
        return ok;
    }

    explicit operator bool() const { return cdb_fileno(&m_db)>=0; }
};

template <typename T>
std::optional<T> unpack( cdb_record data) {
    io::stream<io::array_source> value( static_cast<const char*>(data.data), data.datalen );
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
        throw std::runtime_error( "unpack: unknown compression flag");
    }
    s.push( value );
    T t;
    s >> t;
    if (value.tellg() != static_cast<unsigned>(data.datalen) ) return {};
    return t;
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
    static_assert( sizeof( uid_t ) == 4 , "sizeof(uid_t)!=4");
    buffer.emplace_back( uid & 0xff );
    buffer.emplace_back( ( uid >> 8 ) & 0xff );
    buffer.emplace_back( ( uid >> 16 ) & 0xff );
    buffer.emplace_back( ( uid >> 24 ) & 0xff );
    buffer.emplace_back( t & 0xff );
    buffer.emplace_back( ( t >> 8 ) & 0xff );
    buffer.emplace_back( ( t >> 16 ) & 0xff );
    buffer.emplace_back( ( t >> 24 ) & 0xff );
    std::copy_n( begin( str ), str.size(), back_inserter(buffer) );
    return buffer;
}

template <typename Action>
class Listener : public implements<IIncidentListener> {
public:
   template <typename F>
   Listener(std::string incident, F&& f)
      : m_incident{std::move(incident)}, m_f{std::forward<F>(f)}
   { addRef(); }// Initial count set to 1

   void handle(const Incident& i) override
   { if (i.type() == m_incident) m_f(); }

private:
   /// incident to handle
   const std::string m_incident;
   /// action to perform on incident
   Action m_f;
};

template <typename Fun>
auto make_unique_listener(std::string incident, Fun&& fun)
{ return std::make_unique<Listener<Fun>>(std::move(incident), std::forward<Fun>(fun)); }

}

namespace {
  const std::regex s_optRegex{"(['\"]?)(?:Read(Only|Write))\\1"};
}

namespace ConfigCDBAccessSvc_details {

StatusCode parse(ConfigCDBAccessSvc_details::Mode& result, const std::string& input) {
  if ( std::smatch m; std::regex_match( input, m, s_optRegex ) ) {
    result = ( m[2] == "Only" ) ? Mode::ReadOnly : Mode::ReadWrite;
    return StatusCode::SUCCESS;
  }
  return StatusCode::FAILURE;
}

static const std::array<std::string,2> mode_string{ "ReadOnly", "ReadWrite" };

const std::string& toString(const Mode& mode) {
    switch(mode) {
        case Mode::ReadOnly:  return mode_string[0];
        case Mode::ReadWrite: return mode_string[1];
        default: throw std::runtime_error("unknown CDB file mode");
    }
}

class CDB
{
  public:
    explicit CDB( const std::string& name, std::ios::openmode mode = std::ios::in )
       : m_icdb( name )
    {
        if ( mode & std::ios::out ) {
           m_ocdb.emplace( fs::unique_path( name + "-%%%%-%%%%-%%%%-%%%%" ).string() );
        }
        // if not writing, then inability to open is a fatal error!
        if ( !m_icdb && !m_ocdb ) {
           throw std::runtime_error( "Error opening file " + name + ": " + strerror(errno) );
        }
        // when writing, copy the input if there is a valid input .cdb file
        if ( m_ocdb && m_icdb ) {
           for (auto key_value : m_icdb) m_ocdb->insert(key_value);
        }
    }
    ~CDB() {
        if ( m_ocdb && m_ocdb->close() ) {
           fs::rename( m_ocdb->name(), m_icdb.name() );
        }
    }

    template <typename T>
    std::optional<T> read( std::string_view key )
    {
        // first check input database -- if it is available
        if ( m_icdb ) {
            auto i = m_icdb.find(key);
            if ( i != m_icdb.end() ) {
                auto data = *i;
                if ( data.string_key() != key ) throw std::runtime_error( "Key Mismatch!" );
                return unpack<T>(data);
            }
        }
        // not in input -- when writing, must check write cache!
        if ( m_ocdb ) {
            auto i = m_write_cache.find( std::string{key} );
            if ( i != m_write_cache.end() ) {
                std::stringstream s( i->second );
                T t;
                s >> t;
                return t;
            }
        }
        return {};
    }

    template <typename SELECTOR>
    std::vector<std::string> files( const SELECTOR& selector )
    {
        std::vector<std::string> keys;
        if ( m_icdb ) {
            for (auto key_value : m_icdb ) {
                if ( selector( key_value.string_key() ) ) {
                    keys.emplace_back( key_value.string_key() );
                }
            }
        }
        if ( m_ocdb ) { // then also check write cache...
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

    bool append( std::string_view key, std::stringstream& is )
    {
        if ( !m_ocdb || m_error ) return false;
        // first, look in input database
        auto rd = read<std::string>(key);
        if (rd) {
            if ( rd.value() != is.str() ) {
                m_error = true;
                throw std::runtime_error("CDB append error -- hash collision with file??");
            }
            return true;
        }
        // if not there, look in write cache
        auto i = m_write_cache.find( std::string{key} );
        if ( i != m_write_cache.end() ) {
            bool ok = ( i->second == is.str() );
            if ( !ok ) {
                m_error = true;
                throw std::runtime_error("CDB append error -- hash collision with cache??");
            }
            return ok;
        }
        // aha, this is an as yet unknown key... insert it!
        m_write_cache.emplace( key, is.str() );
        m_ocdb->insert( key, make_cdb_record( is.str(), getUid(), std::time(nullptr) ) );
        return true;
    }
    bool operator!() const { return m_error; }

    const std::string& name() const { return m_icdb.name(); }

  private:
    icdb                m_icdb;
    std::optional<ocdb> m_ocdb;
    std::unordered_map<std::string, std::string> m_write_cache; // write cache..
    mutable uid_t m_myUid = 0;
    bool m_error = false;
};

}
using ConfigCDBAccessSvc_details::CDB;
using ConfigCDBAccessSvc_details::Mode;

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
       m_initListener = make_unique_listener( m_incident, [=](){ this->m_file.reset(); } );
       auto incSvc = service<IIncidentSvc>("IncidentSvc");
       incSvc->addListener(m_initListener.get(), m_incident, std::numeric_limits<long>::max());
    }
    return status;
}

// TODO: return a shared_ptr which keeps the file alive for the duration of the read/write...
ConfigCDBAccessSvc_details::CDB* ConfigCDBAccessSvc::file() const
{
    if ( UNLIKELY(!m_file) ) {
      std::unique_lock<std::mutex> lock(m_file_mtx);
      if (LIKELY(!m_file)){
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
        info() << " opening " << name << " in mode " << toString(m_mode) << endmsg;
        try {
            m_file = std::make_unique<CDB>( name, ( m_mode == Mode::ReadWrite
                                                  ? ( std::ios::in | std::ios::out )
                                                  :   std::ios::in ) );
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
    if (m_initListener) {
       auto incSvc = service<IIncidentSvc>("IncidentSvc");
       if (incSvc) {
          incSvc->removeListener(m_initListener.get(), m_incident);
       }
    }
    m_file.reset( ); // close file if still open
    return Service::finalize();
}

template <typename T>
std::optional<T> ConfigCDBAccessSvc::read( std::string_view path ) const
{
    if ( msgLevel( MSG::DEBUG ) ) debug() << "trying to read " << path << endmsg;
    auto f = file();
    return f ? f->read<T>(path) : std::optional<T>{} ;
}

template <typename T>
bool ConfigCDBAccessSvc::write( std::string_view path, const T& object ) const
{
    auto f = file();
    if ( !f ) {
        debug() << "no open file!" << endmsg;
        return false;
    }
    auto current = f->read<T>( path );
    if ( current ) {
        if ( object == current.value() ) return true;
        error() << " object @ " << path
                << "  already exists, but contents are different..." << endmsg;
        return false;
    }
    if ( m_mode.value() != Mode::ReadWrite ) {
        error() << "attempted write, but file has not been opened ReadWrite" << endmsg;
        return false;
    }
    std::stringstream s;
    s << object;
    try {
        return f->append( path, s );
    } catch (std::runtime_error& err) {
        error() << "failure during write: " << err.what() << endmsg;
        return false;
    }
}

std::optional<PropertyConfig>
ConfigCDBAccessSvc::readPropertyConfig( const PropertyConfig::digest_type& ref )
{
    return this->read<PropertyConfig>( propertyConfigPath( ref ) );
}

std::optional<ConfigTreeNode>
ConfigCDBAccessSvc::readConfigTreeNode( const ConfigTreeNode::digest_type& ref )
{
    return this->read<ConfigTreeNode>( configTreeNodePath( ref ) );
}

std::optional<ConfigTreeNode> ConfigCDBAccessSvc::readConfigTreeNodeAlias(
    const ConfigTreeNodeAlias::alias_type& alias )
{
    std::string fnam = configTreeNodeAliasPath( alias );
    auto sref = this->read<std::string>( fnam );
    if ( !sref ) return {};
    auto ref = ConfigTreeNode::digest_type::createFromStringRep( *sref );
    if ( !ref.valid() ) {
        error() << "content of " << fnam << " not a valid ref" << endmsg;
        return {};
    }
    return readConfigTreeNode( ref );
}

std::vector<ConfigTreeNodeAlias>
ConfigCDBAccessSvc::configTreeNodeAliases( const ConfigTreeNodeAlias::alias_type& alias )
{
    std::vector<ConfigTreeNodeAlias> x;

    auto f = file();
    if ( !f ) return x;
    for ( const auto& i : f->files( PrefixFilenameSelector( "AL/" + alias.major() ) )) {
        if ( msgLevel( MSG::DEBUG ) )
            debug() << " configTreeNodeAliases: adding file " << i << endmsg;
        x.push_back( make_ConfigTreeNodeAlias( f->read<std::string>( i ).value(),
                                               i.substr( 3 ) ) ); // strip leading "AL/"
        if ( msgLevel( MSG::DEBUG ) )
            debug() << " configTreeNodeAliases: content:" << x.back() << endmsg;
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
    auto f = file();
    if ( !f ) {
        error() << " container file not found during attempted write of "
                << alias ;
        return ConfigTreeNodeAlias::alias_type();
    }
    // verify that we're pointing at an existing ConfigTreeNode
    if ( !readConfigTreeNode( alias.ref() ) ) {
        error() << " Alias points at non-existing entry " << alias.ref()
                << "... refusing to create." << endmsg;
        return ConfigTreeNodeAlias::alias_type();
    }
    // now write alias...
    fs::path fnam = configTreeNodeAliasPath( alias.alias() );
    auto x = f->read<std::string>( fnam.string() );
    if (x) {
        //@TODO: decide policy: in which cases do we allow overwrites of existing
        //labels?
        // (eg. TCK aliases: no!, tags: maybe... , toplevel: impossible by
        // construction )
        // that policy should be common to all implementations, so move to a mix-in
        // class,
        // or into ConfigTreeNodeAlias itself
        if ( ConfigTreeNodeAlias::digest_type::createFromStringRep( *x ) ==
             alias.ref() ) {
            return alias.alias();
        }
        error() << " Alias already exists, but contents differ... refusing to change"
                << endmsg;
        return ConfigTreeNodeAlias::alias_type();
    }
    std::stringstream s;
    s << alias.ref();
    if ( !f->append( fnam.string(), s ) ) {
        error() << " failed to write " << fnam.string() << endmsg;
        return ConfigTreeNodeAlias::alias_type();
    }
    info() << " created " << fnam.string() << endmsg;
    return alias.alias();
}
