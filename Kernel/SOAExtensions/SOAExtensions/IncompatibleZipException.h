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
#include <string_view>

namespace Zipping {
  class IncompatibleZipException final : public std::exception {
    std::string m_message{"unspecified failure"};

  public:
    IncompatibleZipException() = default;
    IncompatibleZipException( std::string_view&& s ) : m_message( s ) {}
    std::string message() { return m_message; }
  };
} // namespace Zipping

#endif
