#ifndef DETDESCCNV_XMLCNVSVC_H
#define DETDESCCNV_XMLCNVSVC_H

#include <functional>
#include <string>
#include <vector>

#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/ConversionSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/System.h"
#include "GaudiKernel/extends.h"
#include "XmlTools/Evaluator.h"
#include "XmlTools/IXmlParserSvc.h"
// Include files
#include "XmlTools/IXmlSvc.h"

class IOVDOMDocument;
class IOpaqueAddress;
class ISvcLocator;
struct IXmlSvc;


/** @class XmlCnvSvc XmlCnvSvc.h DetDescCnv/XmlCnvSvc.h
 *
 *  A conversion service for XML based data. It is based on the generic
 *  conversion service and implements in addition the IXmlSvc interface.
 *
 *  @author Sebastien Ponce
 *  @author Radovan Chytracek
 *  @author Pere Mato
 *  @author Marco Clemencic
 */
class XmlCnvSvc : public extends<ConversionSvc,IXmlSvc> {

public:
  /**
   * Standard Constructor
   * @param name String with service name
   * @param svc Pointer to service locator interface
   * @return Reference to CdfPersCnvSvc
   */
  XmlCnvSvc (const std::string& name, ISvcLocator* svc)
      : base_class(name, svc, XML_StorageType) {}

  /**
   * Initializes the service
   * @return status depending on the completion of the call
   */
  StatusCode initialize() override;

  /**
   * Reinitializes the service
   * @return status depending on the completion of the call
   */
  StatusCode reinitialize() override;

  /**
   * Finalizes the service
   * @return status depending on the completion of the call
   */
  StatusCode finalize() override;


  /**
   * Create an XML address using explicit arguments to identify a single object
   * @param svc_type the service type
   * @param clid the CLID of the XML Element for which an address is created
   * @param par an array of two strings containing the dbname and objectname
   * in this order
   * @param refpAddress the new address created
   * @return a StatusCode giving the status of the address creation
   * @note unsigned long fourth argument can be ignored for XML addresses
   */
  using ConversionSvc::createAddress;
  StatusCode createAddress(long  svc_type,
                           const CLID& clid,
                           const std::string* par,
                           const unsigned long* /*ip*/,
                           IOpaqueAddress*& refpAddress) override;


  ///////////////////////////////////////////////////
  // implementation of the IXmlParserSvc interface //
  ///////////////////////////////////////////////////

  /**
   * This method parses an xml file and produces the corresponding DOM
   * document.
   * @param fileName the name of the file to parse
   * @return the document issued from the parsing
   */
  IOVDOMDocument* parse (const char* fileName) override;

  /**
   * This method parses XML from a string and produces the corresponding DOM
   * document.
   * @param source the string to parse
   * @return the document issued from the parsing
   */
  IOVDOMDocument* parseString (std::string source) override;

  /**
   * This clears the cache of previously parsed xml files.
   */
  void clearCache() override;

  /// Method to remove the lock from a document in the cache or to delete the document
  /// generated from a string.
  void releaseDoc(IOVDOMDocument* doc) override;

  /////////////////////////////////////////////
  // implementation of the IXmlSvc interface //
  /////////////////////////////////////////////

  /**
   * Evaluates a numerical expresion
   * @param expr expresion to evaluate. It may include units and parameters
   * @param check boolean to control if the value needs to be check for being a
   * dimentioned magnitude (with units)
   * @return return double value
   */
  double eval( const char* expr, bool check = true ) override;

  /**
   * Evaluates a numerical expresion
   * @param expr expresion to evaluate. It may include units and parameters
   * @param check boolean to control if the value needs to be check for being a
   * dimentioned magnitude (with units)
   * @return return double value
   */
  double eval( const std::string& expr, bool check = true ) override;

  /**
   * Adds a parameter in the list of known parameters. The value can also be an
   * expression with units and other parameters.
   * @param name parameter name
   * @param value string which defines the value of the parameter.
   * @return true if success
   */
  bool addParameter (const std::string& name,
                     const std::string& value) override;

  /**
   * Adds a parameter in the list of known parameters. The value can also be an
   * expression with units and other parameters.
   * @param name parameter name
   * @param value string which defines the value of the parameter.
   * @return true if success
   */
  bool addParameter( const char* name, const char* value ) override;

  /**
   * Adds a parameter in the list of known parameters. The value can also be an
   * expression with units and other parameters.
   * @param name parameter name
   * @param value string which defines the value of the parameter.
   * @return true if success
   */
  bool addParameter( const char* name, double value ) override;

  /**
   * Removes a parameter from the list of known parameters
   * @param name parameter name
   * @return true if success
   */
  bool removeParameter( const std::string& name ) override;

  /**
   * Removes a parameter from the list of known parameters
   * @param name parameter name
   * @return true if success
   */
  bool removeParameter( const char* name ) override;

  /**
   * Accessor to m_genericConversion.
   * Tells whether generic conversion of user defined detector elements should
   * be used in case the corresponding user defined converter is not available
   * @return true if generic conversion is allowed
   */
  bool allowGenericCnv() override { return m_genericConversion; }


private:

// -----------------------------------------------------------------------
// Useful functions to check units in parameters
// -----------------------------------------------------------------------
//
// The aim of all this is to say whether a given expression has a unit
// or not. The language we deal with is the following :
//    sum :=   product '+' sum
//           | product '-' sum
//    product :=   expr '*' product
//               | expr '/' product
//    expr :=   '-'? alphanumeric_token
//            | '-'? function_name? '(' sum ')'
// Where ? means that it is optional and | means or. alphanumeric_token
// contains only alphanumeric characters, except for a single '-' character
// if preceded by a digit and an 'e'
//
// The way it is implemented is the following :
//    - every function receives the string to be parsed and the
//  currently interesting part of it (between start or baseIndex
//  included and end or lastIndex exculded).
//    - the skip* functions return the index of the first character
//  following the sum/product/expr. This value is less or equal to end
//    - the *HasUnit functions tell whether a given sum/product/expr
//  has a unit
//
// The assumptions made are the following :
//    - a sum has a unit if and only if each sub product has one
//    - a product has a unit if any sub expr has one (which may be wrong
//  if you divide a unit by itself for example)
//    - an expression has a unit in only 3 cases :
//         o it is of the form -? '(' sum ')' and the sum has one
//         o it is of the form '-'? alphanumeric token and the token
//  starts with a letter. This statement makes the assumption that this
//  token is either describing a unit or is a variable that has a unit
//  itself, which may be wrong
//         o it is of the form '-'? function_name '(' sum ')' and the function
//  is "sqrt" or "abs" and sum has a unit
//    - note that the result of a function is supposed to have no unit
//  in general

  std::string::size_type skipSum (const std::string& s,
                        std::string::size_type start,
                        std::string::size_type end);
  std::string::size_type skipProduct(const std::string& s,
                            std::string::size_type start,
                            std::string::size_type end);
  std::string::size_type skipExpr (const std::string& s,
                         std::string::size_type start,
                         std::string::size_type end);

  bool sumHasUnit (const std::string& s,
                   std::string::size_type baseIndex,
                   std::string::size_type lastIndex);
  bool productHasUnit (const std::string& s,
                       std::string::size_type baseIndex,
                       std::string::size_type lastIndex);
  bool exprHasUnit (const std::string& s,
                    std::string::size_type baseIndex,
                    std::string::size_type lastIndex);


private:

  /// Name of the XmlParserSvc to use (option XmlCnvSvc.XmlParserSvc, default "XmlParserSvc")
  Gaudi::Property<std::string> m_parserSvcName { this, "XmlParserSvc", "XmlParserSvc" };

  /// XmlParserSvc used to parse xmlfiles
  SmartIF<IXmlParserSvc> m_parserSvc;

  /// Numerical expressions parser
  XmlTools::Evaluator m_xp;

  /**
   * Property triggering generic conversion of user defined detector
   * elements in case the corresponding user defined converter is not
   * available
   */
  Gaudi::Property<bool>  m_genericConversion { this, "AllowGenericConversion", false};


  /**
   * Property specifying the directory containing standard DTD files.
   * This is necessary when parsing XML strings rather than XML files.
   */
  Gaudi::Property<std::string> m_dtdLocation { this, "DtdLocation" };


  /**
   * tells whether to check parameters for units or not.
   */
  Gaudi::Property<bool> m_checkUnits { this, "CheckUnits",  true };


};

#endif    // DETDESCCNV_XMLCNVSVC_H
