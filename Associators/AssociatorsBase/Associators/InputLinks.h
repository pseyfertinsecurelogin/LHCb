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
#ifndef ASSOCIATORS_INPUTLINKS_H
#define ASSOCIATORS_INPUTLINKS_H

#include <iostream>
#include <type_traits>

#include <GaudiKernel/LinkManager.h>
#include <GaudiKernel/GaudiException.h>
#include <GaudiKernel/IRegistry.h>
#include <Event/LinksByKey.h>
#include <Relations/RelationWeighted2D.h>

// template <class Source, class Targed, class Enabled = void>
// class InputLinks {};

namespace {

   template <typename Source>
   struct GetObject
   {
      const Source* operator()(const LHCb::LinksByKey& links,
                               const int linkID, int index) const
      {
         if (linkID != -1) {
            LinkManager::Link* link = links.linkMgr()->link(linkID);
            if (!link) return nullptr;
            ObjectContainerBase* parent = dynamic_cast<ObjectContainerBase*>(link->object());
            return parent ? static_cast<const Source*>(parent->containedObject(index)) : nullptr;
         } else {
            return nullptr;
         }
      }
   };
}

template <class Source, class Target>
class InputLinks {
public:

   using Relations = LHCb::RelationWeighted2D<Source, Target, double>;
   using KeyRelations = LHCb::RelationWeighted2D<unsigned int, Target, double>;

   /// Standard constructor
   InputLinks(const LHCb::LinksByKey& links)
   {
      //== Check proper template, only if specified.
      if (links.sourceClassID() != Source::classID() &&
          CLID_ContainedObject != Source::classID()) {
         std::ostringstream message;
         message << "Incompatible Source type; template classID is "
                 << Source::classID() << " expected " << links.sourceClassID();
         throw GaudiException(message.str(), "LinkedTo", StatusCode::FAILURE);
      }
      if (links.targetClassID() != Target::classID()) {
         std::ostringstream message;
         message << "Incompatible Target type; template classID is " << Target::classID()
                 << " expected " << links.targetClassID();
         throw GaudiException(message.str(), "LinkedTo", StatusCode::FAILURE);
      }

      // Very ugly, but there seems to be no way around the loading here.
      const_cast<LHCb::LinksByKey&>(links).resolveLinks(links.registry()->dataSvc());

      // Fill the relations table by looping over the links. The
      // keyIndex in the LinksByKey contains pairs of source index and
      // reference index. The reference at reference index contains
      // the link ID of the source and target containers, the index of
      // the target and the weight. The source and target objects can
      // be obtained by getting container from the link manager and
      // then the object at the respective indices.

      // The linkReference also contains information on whether there
      // are more targets associated with a given source key. If
      // nextIndex() returns -1, no more targets are associated,
      // otherwise the returned number is the index of the next target.
      const auto& refs = links.linkReference();

      for ( auto [ srcKey, refIndex ] : links.keyIndex() ) {
         do {
            const auto& ref = refs[refIndex];
            const auto target = GetObject<Target>{}(links, ref.linkID(), ref.objectKey());
            // Fill the object to object relations if we can find the
            // source object.
            if (target && ref.srcLinkID() != -1) {
               const Source* source = GetObject<Source>{}(links, ref.srcLinkID(), srcKey);
               if (source) m_relations.i_push(source, target, ref.weight());
            }
            // Always fill the key relations
            if (target) {
               m_keyRelations.i_push(srcKey, target, ref.weight());
            }
            refIndex = ref.nextIndex();
         } while (refIndex >= 0);
      }
      m_relations.i_sort();
      m_keyRelations.i_sort();
   };

   typename LHCb::RelationWeighted2D<unsigned int, Target, double>::Range from(unsigned int key) const
   {
      auto range = m_keyRelations.relations(key);
      return range;
   }

   typename LHCb::RelationWeighted2D<Source, Target, double>::Range from(const Source* source) const
   {
      return m_relations.relations(source);
   }

   template <typename Object, typename std::enable_if<(!std::is_same<Source, Object>::value
                                                       && std::is_same<ContainedObject, Object>::value)
                                                      || (!std::is_same<ContainedObject, Object>::value
                                                          && std::is_same<Source, Object>::value)>::type>
   typename LHCb::RelationWeighted2D<Source, Target, double>::Range from(const Object* source) const
   {
      return m_relations.relations(static_cast<const Source*>(source));
   }

   typename LHCb::RelationWeighted2D<Source, Target, double>::Range to(const Target* target) const
   {
      return m_relations.inverse().relations(target);
   }

   const typename LHCb::RelationWeighted2D<Source, Target, double>& relations() const
   {
      return m_relations;
   }

   const typename LHCb::RelationWeighted2D<unsigned int, Target, double>& keyRelations() const
   {
      return m_keyRelations;
   }

private:

   Relations m_relations;
   KeyRelations m_keyRelations;

};

#endif
