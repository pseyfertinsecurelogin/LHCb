#include "FTXMLUtils.h"

// Date and time from boost
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <iostream>

namespace {

  static const std::string openComment = "<!-- ";
  static const std::string closeComment = " -->";
}

void FT::XMLUtils::authorComment(std::ostringstream& stream,
                                 const std::string& author) {
  comment(stream, "Author", author);
}

void FT::XMLUtils::tagComment(std::ostringstream& stream,
                              const std::string& tag) {
  comment(stream, "Version", tag);
}

void FT::XMLUtils::descriptionComment(std::ostringstream& stream,
                              const std::string& description) {
  comment(stream, "Description", description);
}

void FT::XMLUtils::comment(std::ostringstream& stream,
                           const std::string& name,
                           const std::string& value){
  stream << openComment << name <<  ": " << value << closeComment << std::endl;
}

void FT::XMLUtils::fullComment(std::ostringstream& stream,
                               const std::string& author,
                               const std::string& tag,
                               const std::string desc){
  authorComment(stream, author);
  tagComment(stream, tag);
  descriptionComment(stream, desc);
}


void FT::XMLUtils::replaceChars(std::string& conString) {
  std::cout << "Add comment " <<std::endl;
  std::string blank = " ";
  replace(conString, ",", blank);
  replace(conString, "[", blank);
  replace(conString, "]", blank);
  replace(conString, "\"/", "\"");
}

void FT::XMLUtils::replace(std::string& conString,
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
