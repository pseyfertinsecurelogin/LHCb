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
/// ===========================================================================
/// $Id: 
/// ===========================================================================
#ifndef CALODET_TELL1PARAM_H 
#define CALODET_TELL1PARAM_H 1
#include <vector>
/// ===========================================================================

// Include files
class DeCalorimeter;

/** @class  Tell1Param Tell1Param.h CaloDet/Tell1Param.h
 *  
 *
 */

class Tell1Param 
{
  
  friend class DeCalorimeter;
  
public:
  
  /** constructor 
      @param num
  */
  Tell1Param( int num );
  
  /// destructor 
  ~Tell1Param();
  
  // getters
  int number()      const { return m_number;  }
  std::vector<int> feCards() const { return m_feCards; }
  // setters
  void addFeCard( int num )   { m_feCards.push_back( num ); }  
  bool readPin() const { return m_isPin          ; }
  void setReadPin(bool pin)  {    m_isPin = pin;  }

private:
  
  int m_number;
  std::vector<int> m_feCards;
  bool m_isPin;
  
};


/// ===========================================================================
#endif // CALODET_TELL1PARAM_H
/// ===========================================================================
