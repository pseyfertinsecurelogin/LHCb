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
#ifndef LINKER_LINKERENTRY_H 
#define LINKER_LINKERENTRY_H 1

/** @class LinkerEntry LinkerEntry.h Linker/LinkerEntry.h
 *  Describe an entry of a relation, expanded to pointer
 *
 *  @author Olivier Callot
 *  @date   2005-01-19
 */
template <class SOURCE, class TARGET >
class LinkerEntry final {
public: 
  /** constructor, with the needed information
   *  @param src     source of the relation
   *  @param tgt     target of the relation
   *  @param weight  weight of the relation
   */
  LinkerEntry( const SOURCE* src, const TARGET* tgt, double weight ){
    m_src = src;
    m_target = tgt;
    m_weight = weight;
  }; 

  /** accessor to the source
   *  @return   the source information
   */
  const SOURCE* from()   const { return m_src; }

  /** accessor to the target
   *  @return   the target information
   */
  const TARGET* to()     const { return m_target;}

  /** accessor to the weight
   *  @return   the weight information
   */
  double  weight() const { return m_weight; }

private:
  const SOURCE* m_src;
  const TARGET* m_target;
  double  m_weight;
};
#endif // LINKER_LINKERENTRY_H
