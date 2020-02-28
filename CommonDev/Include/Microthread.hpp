#pragma once

#include <windows.h>
#include <vector>

// Specify how much memory you expect to need for your main thread stack.
// The main disadvantage to having this large is that you are effectively
// allocating that much memory. Don't specify a number larger than the
// amount of address space reserved for your stack - one megabyte by default.
const int k_mainThreadStackSize = 120000;
const int k_stackUsagePerFunction = 1000;

class Microthread
{

public:
   typedef BYTE StackType;

   Microthread(void);
   ~Microthread(void);

   bool update();
   virtual void thread_proc() = 0;
   void wait_one_frame();

private:
   bool createMicroThread();
   void microThreadSleep();

   void markAsDead()
   {
      m_dead = true;
   }
   bool  m_dead;
   std::vector<StackType>  m_stack; // Copy of the script's stack at sleep time.

   static void __cdecl threadFunction(void *pData);
   static void __cdecl threadReturn();

   void preserveStack(std::vector<StackType>& StackStorage);
   void restoreStack(std::vector<StackType>& StackStorage);

   static StackType* s_stack_top;
   static Microthread* s_activeThread;
   static StackType* m_globalStackPointer;



   // Temporarily turn off optimizations, because otherwise this function
   // will be optimized away completely.
#pragma optimize( "", off )
   char* m_uselessBuffer;
   void PrimeStack(int x)
   {
      // Use up big chunks of stack.
      char  unusedArray[k_stackUsagePerFunction];
      m_uselessBuffer = unusedArray;  // Avoid unreferenced local variable warning.
      if (x > 0)
         PrimeStack(x - 1);
   }
#pragma optimize( "", on ) 
};
