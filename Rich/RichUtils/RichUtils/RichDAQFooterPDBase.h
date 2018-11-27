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

//=============================================================================================
/** @file RichDAQFooterPDBase.h
 *
 *  Header file for RICH DAQ utility class : Rich::DAQ::FooterPDBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/09/2006
 */
//=============================================================================================

#pragma once

// Gaudi
#include "GaudiKernel/GaudiException.h"

// numberings
#include "RichUtils/RichDAQDefinitions.h"

// Kernel
#include "Kernel/FastAllocVector.h"
#include "Kernel/MemPoolAlloc.h"

namespace Rich::DAQ
{

  //=============================================================================================
  /** @class FooterPDBase RichDAQFooterPDBase.h
   *
   *  Base class for RICH HPD footers. Implements basic common functions
   *
   *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *  @date   05/09/2006
   */
  //=============================================================================================
  class FooterPDBase
  {

  public:

    /// Type for a single word. Strongly typed for safety.
    using WordType = NumericType< LongType >;
    /// Type for header words
    using FooterWords = LHCb::STL::Vector< WordType >;

  public:

    /// Constructor with number of header words
    explicit FooterPDBase( const ShortType nWords, const WordType wordInit = WordType( 0 ) )
      : m_footerWords( nWords, wordInit )
    {}

    /// Constructor from raw footer word(s)
    explicit FooterPDBase( const FooterWords &words ) : m_footerWords( words ) {}

  public:

    /// Default constructor ( 0 words in footer )
    FooterPDBase() = default;

  public: // methods

    /// Read only access to footer words
    inline const FooterWords &footerWords() const & { return m_footerWords; }

    /// Move access to footer words
    inline FooterWords &&footerWords() && noexcept { return std::move( m_footerWords ); }

    /// Returns the number of data words in the footer
    inline FooterWords::size_type nFooterWords() const { return footerWords().size(); }

  public: // reading and writing

    /// Read given number of data words from stream
    /// Note, after this call data pointer is incremented to the next word after the footer
    inline void readFromDataStream( const LongType *&data )
    {
      for ( FooterWords::size_type i = 0; i < nFooterWords(); ++i )
      { footerWords()[i] = WordType( *( data++ ) ); }
    }

    /// Write this head to a RAWBank of data words
    inline void fillRAWBank( RAWBank &rawData ) const
    {
      for ( const auto &F : footerWords() ) { rawData.push_back( F.data() ); }
    }

  protected: // methods

    /// Set the data value using the given mask and shift values
    inline bool set( const ShortType value, const ShortType shift, const LongType mask )
    {
      footerWords()[0] =
        WordType( ( ( value << shift ) & mask ) | ( footerWords()[0].data() & ~mask ) );
      return true;
    }

    /// tests whether a given value is in range for a given data field
    inline bool dataInRange( const ShortType value, const ShortType max ) const
    {
      return ( value <= max );
    }

    /// Read/Write access to footer words
    inline FooterWords &footerWords() & { return m_footerWords; }

    /// Set the given footer word
    void setWord( const ShortType word, const WordType data ) { footerWords()[word] = data; }

  private: // data

    /// The footer words
    FooterWords m_footerWords;
  };

} // namespace Rich::DAQ
