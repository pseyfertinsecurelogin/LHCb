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
#include "Kernel/STXMLUtils.h"

// Date and time from boost
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <iostream>

namespace {

  static const std::string openComment = "<!-- ";
  static const std::string closeComment = " -->";
}

void ST::XMLUtils::authorComment(std::ostringstream& stream,
                                 const std::string& author) {
  comment(stream, "Author", author);
}

void ST::XMLUtils::tagComment(std::ostringstream& stream,
                              const std::string& tag) {
  comment(stream, "Version", tag);
}

void ST::XMLUtils::descriptionComment(std::ostringstream& stream,
                              const std::string& description) {
  comment(stream, "Description", description);
}

void ST::XMLUtils::comment(std::ostringstream& stream,
                           const std::string& name,
                           const std::string& value){
  stream << openComment << name <<  ": " << value << closeComment << std::endl;
}

void ST::XMLUtils::fullComment(std::ostringstream& stream,
                              const std::string& author,
                              const std::string& tag,
		              const std::string desc){
  authorComment(stream, author);
  tagComment(stream, tag);
  descriptionComment(stream, desc);
}


void ST::XMLUtils::replaceChars(std::string& conString) {
  std::cout << "Add comment " <<std::endl;
  std::string blank = " ";
  replace(conString, ",", blank);
  replace(conString, "[", blank);
  replace(conString, "]", blank);
  replace(conString, "\"/", "\"");
}

void ST::XMLUtils::replace(std::string& conString,
                           std::string in,
                           std::string out ) {

  std::string::size_type pos = 0;
  while (pos != std::string::npos){
    pos = conString.find(in,pos);
    if (pos != std::string::npos) {
      conString.replace(pos,in.size(),out);
    }
  } //pos
}
