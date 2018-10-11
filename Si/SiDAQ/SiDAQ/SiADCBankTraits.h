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
#ifndef SIDAQ_SIADCBANKTRAITS_H 
#define SIDAQ_SIADCBANKTRAITS_H 1


namespace SiDAQ {

  /** @struct adc_only_bank_tag SiADCBankTraits.h SiDAQ/SiADCBankTraits.h
   *
   *  Flags raw adc bank without neighbour sum byte.
   *
   *  This class is used implement a traits pattern needed to
   *  distinguish ST and Velo raw ADC banks at compile time.
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-21
   */
  struct adc_only_bank_tag {};

  /** @struct adc_neighboursum_bank_tag SiADCBankTraits.h SiDAQ/SiADCBankTraits.h
   *
   *  Flags raw adc bank with neighbour sum byte.
   *
   *  This class is used implement a traits pattern needed to
   *  distinguish ST and Velo raw ADC banks at compile time.
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-21
   */
  struct adc_neighboursum_bank_tag {};

}
#endif // SIDAQ_SIADCBANKTRAITS_H
