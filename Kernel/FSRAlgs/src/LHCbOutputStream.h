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
#ifndef LHCB_OUTPUTSTREAM_H
#define LHCB_OUTPUTSTREAM_H

// Required for inheritance
#include "GaudiKernel/IDataSelector.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/IRegistry.h"

// STL include files
#include <memory>
#include <vector>
#include <string>

// forward declarations
class IConversionSvc;
struct IDataManagerSvc;
class DataStoreItem;

/** A small to stream Data I/O.
    Author:  M.Frank
    author R. Lambert
    Version: 1.0
*/
class LHCbOutputStream : public Algorithm     {
public:
  typedef std::vector<DataStoreItem*> Items;
  typedef std::vector<std::string>    ItemNames;
  typedef std::map< Algorithm*,  Items     > AlgDependentItems;
  typedef std::map< std::string, ItemNames > AlgDependentItemNames;
protected:
  /// Reference to the incident service
  SmartIF<IIncidentSvc> m_incidentSvc;
  /// Flag indicating whether data pre-loading should be performed
  Gaudi::Property<bool> m_doPreLoad { this,"Preload", true };
  /// Flag indicating whether optional items should be preloaded
  Gaudi::Property<bool> m_doPreLoadOpt { this,"PreloadOptItems", false };
  /// Flag to indicate that item consistency should be checked
  Gaudi::Property<bool> m_verifyItems { this,"VerifyItems", true };
  /// Name of the service managing the data store
  Gaudi::Property<std::string> m_storeName{ this, "EvtDataSvc", "EventDataSvc" } ;
  /// Name of the persistency service capable to write data from the store
  Gaudi::Property<std::string> m_persName{ this, "EvtConversionSvc", "EventPersistencySvc" } ;
  /// Name of the output file specification
  Gaudi::Property<std::string> m_output{ this, "Output" } ;
  /// Name of the output file
  Gaudi::Property<std::string> m_outputName{ this, "OutputFile" } ;
  /// Output type: NEW(NEW,CREATE,WRITE,RECREATE), UPDATE)
  std::string m_outputType = "UPDATE";
  /// Keep reference to the data provider service
  SmartIF<IDataProviderSvc>        m_pDataProvider;
  /// Keep reference to the data manager service
  SmartIF<IDataManagerSvc>         m_pDataManager;
  /// Keep reference to the data conversion service
  SmartIF<IConversionSvc>          m_pConversionSvc;
  /// Keep track of the current item
  DataStoreItem*           m_currentItem;
  /// Vector of item names
  Gaudi::Property<ItemNames> m_itemNames{ this, "ItemList" };
  /// Vector of items to be saved to this stream
  Items                    m_itemList;
  /// Vector of item names
  Gaudi::Property<ItemNames> m_optItemNames{ this, "OptItemList" };
  /// Vector of optional items to be saved to this stream
  Items                    m_optItemList;
  /** Mapping between algorithm names, and a list of items for which, if the
   *  algorithm in question accepted the event, they should be also stored. */
  Gaudi::Property<AlgDependentItemNames> m_algDependentItemList{ this, "AlgDependentItemList" };
  /// Items to be saved for specific algorithms
  AlgDependentItems        m_algDependentItems;
  /// Collection of objects being selected
  IDataSelector            m_objects;
  /// Number of events written to this output stream
  int                      m_events;
  /// Vector of names of Algorithms that this stream accepts
  Gaudi::Property<std::vector<std::string>> m_acceptNames{ this, "AcceptAlgs", {},
      [=](auto&) {
          if ( auto sc = decodeAlgorithms(m_acceptNames, &m_acceptAlgs ); sc.isFailure()) {
                throw GaudiException("Failure in LHCbOutputStream::decodeAlgorithms",
                                     "LHCbOutputStream::acceptAlgsHandler",sc);
      } ; } };

  /// Vector of names of Algorithms that this stream requires
  Gaudi::Property<std::vector<std::string>> m_requireNames{ this, "RequireAlgs", {},
       [=](auto&) {
            if ( auto sc = decodeAlgorithms( m_requireNames, &m_requireAlgs ); sc.isFailure()) {
                throw GaudiException("Failure in LHCbOutputStream::decodeAlgorithms",
                                     "LHCbOutputStream::requireAlgsHandler",sc);
       } ; } } ;
  /// Vector of names of Algorithms that this stream is vetoed by
  Gaudi::Property<std::vector<std::string>> m_vetoNames{ this, "VetoAlgs", {},
       [=](auto&) {
             if (auto sc = decodeAlgorithms( m_vetoNames, &m_vetoAlgs ); sc.isFailure()) {
                throw GaudiException("Failure in LHCbOutputStream::decodeAlgorithms",
                                     "LHCbOutputStream::vetoAlgsHandler",sc);
       } ; } };
  /// Vector of Algorithms that this stream accepts
  std::vector<Algorithm*>  m_acceptAlgs;
  /// Vector of Algorithms that this stream requires
  std::vector<Algorithm*>  m_requireAlgs;
  /// Vector of Algorithms that this stream is vetoed by
  std::vector<Algorithm*>  m_vetoAlgs;

public:
  /// Standard algorithm Constructor
  using Algorithm::Algorithm;
protected:
  /// Decode list of Algorithms that this stream accepts
  StatusCode decodeAcceptAlgs();
  /// Decode list of Algorithms that this stream requires
  StatusCode decodeRequireAlgs();
  /// Decode list of Algorithms that this stream is vetoed by
  StatusCode decodeVetoAlgs();
  /// Handler for VetoAlgs Property
  void vetoAlgsHandler( Property& theProp );
  /// Decode a single algorithm name
  Algorithm* decodeAlgorithm( const std::string& theName );
  /// Decode specified list of Algorithms
  StatusCode decodeAlgorithms( StringArrayProperty& theNames,
                               std::vector<Algorithm*>* theAlgs );
  /// Test whether this event should be output
  bool isEventAccepted() const;
  /// Find single item identified by its path (exact match)
  DataStoreItem* findItem(const std::string& path);
  /// Select the different objects and write them to file
  virtual StatusCode writeObjects();

  ///should I fire incidents for writing opening/closing etc?
  ///in the baseclass, always fire the incidents by default
  ///in RecordStream this will be set to false, and configurable
  bool m_fireIncidents = true;

  /// Tell if the instance has been configured with input items or not.
  virtual bool hasInput() const;

public:
  /// Initialize LHCbOutputStream
  StatusCode initialize() override;
  /// Terminate LHCbOutputStream
  StatusCode finalize() override;
  /// Working entry point
  StatusCode execute() override;
  // Connect to proper conversion service
  virtual StatusCode connectConversionSvc();
  /// Store agent's classback
  virtual bool collect(IRegistry* dir, int level);
  /// Collect all objects to be written to the output stream
  virtual StatusCode collectObjects();
  /// Add item to output streamer list
  void addItem(Items& itms, const std::string& descriptor);
  /// Clear list of selected objects
  void clearSelection();
  /// Return the list of selected objects
  IDataSelector* selectedObjects() { return &m_objects; }
};

#endif // LHCB_OUTPUTSTREAM_H
