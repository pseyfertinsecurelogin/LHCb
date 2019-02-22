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
// $Id: VeloRawWordSizes.h,v 1.1 2006-03-07 15:53:00 krinnert Exp $
#ifndef VELORAWWORDSIZES_H
#define VELORAWWORDSIZES_H 1

#include "SiDAQ/SiRawBufferWord.h"
#include <cstddef>

namespace VeloDAQ {
  // the size of a cluster word in bytes
  const size_t clu_word_size = sizeof( unsigned short );
  // the number of cluster words per buffer word
  const size_t clu_per_buffer = sizeof( SiDAQ::buffer_word ) / clu_word_size;
  // the power of two that yields the number of bits in a cluster word,
  // needed for shifting
  const size_t clu_shift = 4;

  // the size of an adc word in bytes
  const size_t adc_word_size = sizeof( unsigned char );
  // the number of adc words per buffer word
  const size_t adc_per_buffer = sizeof( SiDAQ::buffer_word ) / adc_word_size;
  // the power of two that yields the number of bits in an adc word,
  // needed for shifting
  const size_t adc_shift = 3;
} // namespace VeloDAQ

#endif // VELORAWWORDSIZES_H
