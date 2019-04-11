/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#ifndef IncompatibleZipException_H
#define IncompatibleZipException_H 1
#include <exception> // IWYU pragma: keep
// IWYU pragma: no_include <bits/exception.h>
#include <string>

namespace Zipping {
  /** @class IncompatibleZipException
   *
   * @brief Exception that's thrown when zipping Zipping::ZipContainers that are semantically incompatible
   */
  class IncompatibleZipException final : public std::exception {
    std::string m_message{"unspecified failure"}; ///< message in the exception with more details

  public:
    /// default constructor deleted, intention is to always provide details
    IncompatibleZipException() = default;
    /// constructor with additional method
    IncompatibleZipException( std::string s ) : m_message( std::move( s ) ) {}
    /// print the contained message with details
    std::string message() { return m_message; }
  };
} // namespace Zipping

#endif
