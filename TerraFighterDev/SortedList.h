#ifndef __SortedList_H__
#define __SortedList_H__

template <class TYPE>
bool Compare(TYPE a,TYPE b)
{
   return a < b;
}

typedef BOOL (CALLBACK* PROCESSPROC2)( void*,  void*);

template <class TYPE>
class CSortedList
{
public:
   CSortedList()
   {
      m_pItems = NULL;
      m_pUnused = NULL;
   }
   void Add(TYPE val)
   {
      if(m_pItems == NULL)
         m_pItems = CreateItem(val);
      else
      {
         if(Compare(m_pItems->m_value,val))
         {
            CSLItem* pTemp = m_pItems;
            while(pTemp)
            {
               if(pTemp->m_pNext == NULL)
               {
                  // added at the end of the list
                  pTemp->m_pNext = CreateItem(val);
                  break;
               }
               else
               {
                  if(!Compare(pTemp->m_pNext->m_value,val))
                  {
                     // inserted the item in the list
                     CSLItem* pNewTemp = pTemp->m_pNext;
                     pTemp->m_pNext = CreateItem(val,pNewTemp);
                     break;
                  }
               }
               pTemp = pTemp->m_pNext;
            }
         }
         else
         {
            CSLItem* pTemp = m_pItems;
            m_pItems = CreateItem(val,pTemp);
         }
      }
   }
   void ProcessElements(void* pData,PROCESSPROC2 pFunct)
   {
      CSLItem* pTemp = m_pItems;
      while(pTemp)
      {
         (*pFunct)(pTemp->m_value,pData);
         pTemp = pTemp->m_pNext;
      }
   }

   void Clear()
   {
      if(m_pUnused)
         delete m_pUnused;
      m_pUnused = m_pItems;
      m_pItems = NULL;
   }



   ~CSortedList()
   {
      if(m_pItems)
         delete m_pItems;
      if(m_pUnused)
         delete m_pUnused;
   }
private:

   class CSLItem
   {
   public:
      CSLItem(TYPE val,CSLItem* pNext = NULL)
      {
         m_pNext = pNext;
         m_value = val;
      }
      TYPE m_value;
      CSLItem* m_pNext;
      ~CSLItem()
      {
         if(m_pNext)
            delete m_pNext;
      }
   };

   CSLItem* CreateItem(TYPE val, CSLItem* pNext = NULL)
   {
      CSLItem* pItem = NULL;
      if(m_pUnused)
      {
         pItem = m_pUnused;
         m_pUnused = pItem->m_pNext;
         pItem->m_value=val;
         pItem->m_pNext=pNext;
      }
      else
         pItem = new CSLItem(val,pNext);
      return pItem;
   }

   CSLItem* m_pItems;
   CSLItem* m_pUnused;
};

#endif