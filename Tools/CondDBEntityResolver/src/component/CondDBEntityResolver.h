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
#ifndef CONDDBENTITYRESOLVERTOOL_H_
#define CONDDBENTITYRESOLVERTOOL_H_

// Include files
#include <type_traits>
#include <boost/optional.hpp>
#include <boost/utility/string_ref.hpp>
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IFileAccess.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/Time.h"
#include "XmlTools/IXmlEntityResolver.h"
#include "Kernel/ICondDBInfo.h"

class ICondDBReader;

/** @class CondDBEntityResolver CondDBEntityResolver.h
 *
 *  Class implementing IXmlEntityResolver interface and xercesc::EntityResolver.
 *  An instance of this service can be used to allow inclusion of XML DTDs and documents
 *  from CondDB. It works implementing the EntityResolver interface defined in Xerces-C++,
 *  which allow to intercept the requests of external XML data.\n
 *  Examples of CondDB external references:
 *  \li \<!DOCTYPE DDDB SYSTEM "conddb:/DTD/structure"\>
 *  \li \<!ENTITY Parameters SYSTEM "conddb:/Parameters/SpecialParamters"\>
 *
 *  This tool implements the Gaudi IFileAccess interface, so that it can be used
 *  to retrieve files from the CondDB.
 *
 *  @author Marco Clemencic
 *  @date   2005-10-18
 */
class CondDBEntityResolver: public extends<AlgTool, IXmlEntityResolver, IFileAccess, ICondDBInfo>,
                            virtual public xercesc::EntityResolver {
public:

  CondDBEntityResolver( const std::string& type, const std::string& name, const IInterface* parent );
  ~CondDBEntityResolver() override = default;

  // ---- Overloaded from AlgTool ----

  /// Initialize the tool
  StatusCode initialize() override;

  /// Finalize the tool
  StatusCode finalize() override;


  // ---- Implementation of IXmlEntityResolverSvc -------------

  /// Return a pointer to the actual implementation of a xercesc::EntityResolver.
  xercesc::EntityResolver *resolver() override { return this; }


  // ---- Implementation of xercesc::EntityResolver -------------

  /// Create a Xerces-C input source based on the given systemId (publicId is ignored).
  /// If the systemId does not begin with "conddb:", it returns NULL, so the parser go on with the default action.
  xercesc::InputSource *resolveEntity( const XMLCh *const publicId, const XMLCh *const systemId ) override;


  // ---- Implementation of IFileAccess -------------

  /// Find the URL and returns an auto_ptr to an input stream interface of an
  /// object that can be used to read from the file the URL is pointing to.
  /// Returns an empty pointer if the URL cannot be resolved.
  using open_result_t = typename std::result_of<decltype(&IFileAccess::open)(IFileAccess&,std::string)>::type;
  open_result_t open(const std::string &url) override;

  /// @see IFileAccess::protocols
  const std::vector<std::string> &protocols() const override;

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const override;

private:

  /// Name of the CondDBCnvSvc instance.
  StringProperty m_condDBReaderName{"CondDBReader", "CondDBCnvSvc"};
  /// Name of the DetectorDataService instance (for the event time).
  StringProperty m_detDataSvcName{"DetDataSvc", "DetDataSvc/DetectorDataSvc"};


  /// Return the pointer to the CondDBReader (loading it if not yet done).
  ICondDBReader *condDBReader() const;

  /// Return the pointer to the detector data service (loading it if not yet done).
  IDetDataSvc *detDataSvc();

  /// Fill validity limits and data (str) with the content retrieved from the url
  boost::optional<std::string>  i_getData(boost::string_ref url,
                                          Gaudi::Time &since, Gaudi::Time &until);


  /// Pointer to the CondDBCnvSvc instance.
  mutable SmartIF<ICondDBReader> m_condDBReader;

  /// Pointer to the DetectorDataService instance (for the event time).
  SmartIF<IDetDataSvc> m_detDataSvc;

};
#endif /*CONDDBENTITYRESOLVERTOOL_H_*/
