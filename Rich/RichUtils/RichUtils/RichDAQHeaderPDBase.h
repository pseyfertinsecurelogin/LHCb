
//=============================================================================================
/** @file RichDAQHeaderPDBase.h
 *
 *  Header file for RICH DAQ utility class : Rich::DAQ::HeaderPDBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//=============================================================================================

#pragma once

// Gaudi
#include "GaudiKernel/GaudiException.h"

// numberings
#include "RichUtils/RichDAQDefinitions.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"
#include "Kernel/FastAllocVector.h"

namespace Rich
{
  namespace DAQ
  {

    /** @class HeaderPDBase RichDAQHeaderPDBase.h
     *
     *  Base class for RICH HPD headers. Implements basic common functions
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-06
     */
    class HeaderPDBase
    {

    public:

      /// Type for a single word. Strongly typed for safety.
      using WordType = NumericType<LongType>;
      /// Type for extended header words
      using ExtendedHeaderWords = LHCb::STL::Vector<WordType>;
      /// Type for all header words
      using HeaderWords         = LHCb::STL::Vector<WordType>;

    public:

      /// Constructor with number of header words and init value
      explicit HeaderPDBase ( const ShortType nWords,
                              const WordType  wordInit )
        : m_primaryHeader( wordInit ), 
          m_extendedWords( nWords>0 ? nWords-1 : 0, wordInit ) { }
      
      /// Constructor from primary and extended header words.
      explicit HeaderPDBase( const WordType primWord,
                             const ExtendedHeaderWords & exWords )
        : m_primaryHeader(primWord), m_extendedWords(exWords) { }

      /// Constructor from primary word.
      explicit HeaderPDBase( const WordType primWord )
        : m_primaryHeader(primWord) { }
      
    public:

      /// Default constructor
      HeaderPDBase() = default;

    public: // methods

      /// Read access the primary head word
      inline const WordType& primaryHeaderWord() const noexcept
      {
        return m_primaryHeader;
      }

      /// Read access the extended header words
      inline const ExtendedHeaderWords& extendedHeaderWords() const & noexcept
      {
        return m_extendedWords;
      }

      /// Move access the extended header words
      inline ExtendedHeaderWords&& extendedHeaderWords() && noexcept
      {
        return std::move(m_extendedWords);
      }

      /// Returns the number of data words in the header
      inline ExtendedHeaderWords::size_type nHeaderWords() const noexcept
      {
        return ( WordType(0) != m_primaryHeader ? 1 : 0 ) + m_extendedWords.size();
      }

      /// Returns the number of extended data words in the header
      inline ExtendedHeaderWords::size_type nExtendedHeaderWords() const noexcept
      {
        return m_extendedWords.size();
      }

      /// Get a container of all header words, including the primary one
      inline HeaderWords allHeaderWords() const noexcept
      {
        // words to return
        HeaderWords allWords;
        // reserve enough size
        allWords.reserve( nHeaderWords() );
        // add the words
        allWords.emplace_back( primaryHeaderWord() );
        for ( const auto W : extendedHeaderWords() ) { allWords.emplace_back(W); }
        // return
        return allWords;
      }

    public: // writing

      /// Write this head to a RAWBank of data words
      inline void fillRAWBank ( RAWBank & rawData ) const
      {
        rawData.emplace_back( primaryHeaderWord().data() );
        for ( const auto& H : extendedHeaderWords() ) { rawData.emplace_back(H.data()); }
      }

    protected: // methods

      /// Set the primary header word
      inline void setPrimaryHeaderWord( const WordType word ) noexcept
      {
        m_primaryHeader = word;
      }

      /// Read/Write access to extended header words
      inline ExtendedHeaderWords & extendedHeaderWords() & noexcept 
      {
        return m_extendedWords; 
      }

      /// Set the given header word
      inline void setHeaderWord( const unsigned short iW,
                                 const LongType word )
      {
        if ( 0 == iW ) { m_primaryHeader       = WordType(word); }
        else           { m_extendedWords[iW-1] = WordType(word); }
      }

      /// Set the data value using the given mask and shift values in the primary word
      inline bool set( const ShortType value,
                       const ShortType shift,
                       const LongType  mask )
      {
        m_primaryHeader = WordType( ((value << shift) & mask) | (m_primaryHeader.data() & ~mask) );
        return true;
      }

      /// Set the data value using the given mask and shift values in the given word
      inline bool set( const ShortType value,
                       const ShortType shift,
                       const LongType  mask,
                       const ShortType wordNumber )
      {
        auto & word = ( 0 == wordNumber ? m_primaryHeader : m_extendedWords[wordNumber-1] );
        word = WordType( ((value << shift) & mask) | (word.data() & ~mask) );
        return true;
      }

      /// tests whether a given value is in range for a given data field
      inline bool dataInRange( const ShortType value,
                               const ShortType max ) const noexcept
      {
        return ( value <= max );
      }

    private: // data

      /// The primary header word
      WordType m_primaryHeader{0};

      /// The extended header words
      ExtendedHeaderWords m_extendedWords;

    };

  }
}

