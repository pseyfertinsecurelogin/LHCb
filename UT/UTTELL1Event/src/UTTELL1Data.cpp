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
// fillstream method

#include "Event/UTTELL1Data.h"
#include "Event/UTTELL1EventInfo.h"
#include <iostream>

std::ostream& LHCb::UTTELL1Data::fillStream(std::ostream& s) const{

  s << "Board " <<  TELL1ID() << std::endl;
  s << "Sent PPs " ;
  for (std::vector<unsigned int>::const_iterator iter = m_sentPPs.begin();
       iter != m_sentPPs.end(); ++iter){
    s << *iter << " " << std::endl; 
  } // sent pp
  s << std::endl;
  
  s << "Data header " << std::endl;
  const UTTELL1Data::Data& outputvector_header = header();
  for (int unsigned i=0; i< 24; ++i){	                                                                                       
       s  << "########################################################################" << std::endl;	  
       s  << "Optical link " << i << std::endl;
       s  << "Headers:#" << std::endl;;
       s  << "################################################" << std::endl;;
       s  << "Port 1:    |Port 2:    |Port 3:    |Port 4:    #" << std::endl;;
       s  << "-----------|-----------|-----------|-----------#" << std::endl;;
       s  << outputvector_header[i][0] << " "
	       << outputvector_header[i][1] << " "
	       << outputvector_header[i][2] << "|"
	       << outputvector_header[i][3] << " "
	       << outputvector_header[i][4] << " "
	       << outputvector_header[i][5] << "|"
	       << outputvector_header[i][6] << " "
	       << outputvector_header[i][7] << " "
	       << outputvector_header[i][8] << "|"
	       << outputvector_header[i][9] << " "
	       << outputvector_header[i][10] << " "
	       << outputvector_header[i][11] << "#" << std::endl;
  } // loop links		
  s << "########################################################################" << std::endl;
  return s;
}

std::vector<const LHCb::UTTELL1EventInfo*> LHCb::UTTELL1Data::validData() const{

  std::vector<const LHCb::UTTELL1EventInfo*> theData;
  std::vector<unsigned int> sentPP = sentPPs(); 
  for (std::vector<unsigned int>::const_iterator iter = m_sentPPs.begin();
       iter != m_sentPPs.end(); ++iter){
    const LHCb::UTTELL1EventInfo* info = m_eventInfo[*iter];
    theData.push_back(info);
  }

  return theData;
}


