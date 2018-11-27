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
#ifndef CONDITIONPARSER_H_
#define CONDITIONPARSER_H_

#include "DetDesc/ParamValidDataObject.h"

/** Wrapper around the parser to convert strings to condition objects.
 *  (written with boost::spirit, http://boost.org/libs/spirit/index.html).
 *
 *  @author Marco Clemencic
 *  @date   2006-10-23
 */
bool ConditionParser(const std::string &str, std::string &name, ParamValidDataObject &cond);

#endif /*CONDITIONPARSER_H_*/
