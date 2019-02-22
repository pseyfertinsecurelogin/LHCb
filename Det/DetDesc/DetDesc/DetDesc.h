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
#ifndef DETDESC_DETDESC_H
#define DETDESC_DETDESC_H 1

// includes files
#include "DetDesc/Services.h"
#include <string>

/**
 * @namespace DetDesc DetDesc.h "DetDesc/DetDesc.h"
 *
 * single access point to several usefull methods and accessor.
 * The main goal here is to cache references to very usefull services
 * to avoid a call to Service::service from each small component or
 * object.
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @author Sebastien Ponce
 */
namespace DetDesc {

  /**
   * print double value using format
   * @param value   value to be printed
   * @param format  desirable format
   * @return string representation of the "value"
   */
  std::string print( const double value, const char* format = "%9.3g" );

  /**
   * print  long value using format
   * @param value   value to be printed
   * @param format  desirable format
   * @return string representation of the "value"
   */
  std::string print( const long value, const char* format = "%d" );

  /**
   * Gets an instance of Services
   */
  ServicesPtr services();

} // namespace DetDesc

#endif ///< DETDESC_DETDESC_H
