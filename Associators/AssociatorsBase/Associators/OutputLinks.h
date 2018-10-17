#ifndef ASSOCIATORS_OUTPUTLINKS_H
#define ASSOCIATORS_OUTPUTLINKS_H

#include <Event/LinksByKey.h>
#include <GaudiKernel/LinkManager.h>

template <typename Source, typename Target>
class OutputLinks final {
public:

   OutputLinks()
   {
      m_links.setTargetClassID(Target::classID());
      m_links.setSourceClassID(Source::classID());
   }

   operator LHCb::LinksByKey()
   {
      // This is horrible, but needed because the copy constructor of
      // DataObject creates a new (and empty) LinkManager. The
      // existing move constructor moves the linkmanager, which would
      // work, expect the using auto conversion twice would result in
      // undefined behaviour. So, create a copy, insert all the links
      // it's and then return it.
      LHCb::LinksByKey r{m_links};
      for (long linkID = 0; linkID != m_links.linkMgr()->size(); ++ linkID) {
         auto link = m_links.linkMgr()->link(linkID);
         r.linkMgr()->addLink(link->path(), link->object());
      }
      return r;
   }

   const LHCb::LinksByKey& links() const { return m_links; }
   LHCb::LinksByKey& links() { return m_links; }

   void reset() { m_links.reset(); }

   void link(const Source* source, const Target* dest, double weight = 1.)
   {
      if (!source || !dest) return;

      m_links.addReference(source->index(), m_links.linkID(source->parent()),
                           dest->index(), m_links.linkID(dest->parent()),
                           weight);
   }

   void link(int key, const Target* dest, double weight = 1.)
   {
      if (!dest) return;

      m_links.addReference(key, -1, dest->index(), m_links.linkID(dest->parent()), weight);
   }

   void setIncreasingWeight() { m_links.setIncreasing(); }
   void setDecreasingWeight() { m_links.setDecreasing(); }

private:

   LHCb::LinksByKey m_links;

};

#endif
