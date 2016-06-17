// $Id: $
#ifndef FILESTAGER_FILESTAGERFUNCTIONS_H
#define FILESTAGER_FILESTAGERFUNCTIONS_H 1

#include <string>

namespace FileStager {
   bool createPFN( std::string& remote, std::string& command, bool stageLocal = false );
}

#endif // FILESTAGER_FILESTAGERFUNCTIONS_H
