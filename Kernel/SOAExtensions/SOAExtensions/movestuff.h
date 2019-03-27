  struct ZipFamilyNumber {
    explicit ZipFamilyNumber( uint64_t t ) : m_value( t ) {}
    uint64_t m_value;

    // assuming most of what can be done with uint64_t doesn't make sense as a
    // family identifier I disable implicit conversion.
    explicit operator uint64_t() const { return m_value; }

    bool operator==( const ZipFamilyNumber other ) { return m_value == other.m_value; }
    bool operator!=( const ZipFamilyNumber other ) { return m_value != other.m_value; }
  };

  namespace details {
    class ZipFamilyNumberGenerator {
    private:
      inline static std::atomic_uint64_t s_generator;

    public:
      static ZipFamilyNumber generate() { return ZipFamilyNumber( s_generator.fetch_add( 1 ) ); }
    };
  } // namespace details

  template <typename FIRST, typename... OTHERS>
  ZipFamilyNumber firstid( FIRST& first, OTHERS&... /*unused*/ ) {
    return first.zipIdentifier();
  }
