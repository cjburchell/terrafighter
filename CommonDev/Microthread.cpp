#include "StdAfx.h"
#include "microthread.hpp"

Microthread::StackType* Microthread::s_stack_top = NULL;


Microthread* Microthread::s_activeThread = NULL;
Microthread::StackType* Microthread::m_globalStackPointer = NULL;


Microthread::Microthread(void)
{
   m_dead = false;
   m_uselessBuffer = NULL;
   createMicroThread();
}



Microthread::~Microthread(void)
{
}

bool Microthread::createMicroThread()
{
   static bool s_stackPrimed = false;
   if (!s_stackPrimed)
   {
      // Make the OS commit memory far enough down the stack.
      PrimeStack(k_mainThreadStackSize / k_stackUsagePerFunction);
      int localVariable;
      // Initialize the script stack top to an appropriate location.
      // The current stack pointer is detected by the address of a
      // local variable.
      s_stack_top = ((StackType*)&localVariable) - k_mainThreadStackSize;
      s_stackPrimed = true;
   }
   // All we have to do to initialize a new micro thread is create a plausible initial
   // stack, containing the needed registers. That's it.

   // At the bottom of the stack we have the four registers that we have to preserve and
   // restore. Above that we have the address of our startup function that we fall back
   // into. Once we've returned to that function we want to look like we just called
   // that function, so the next value down is the return address. We put MicroThreadReturn
   // there so that if the users thread routine returns they will automatically call
   // it. Lastly we push on the user specified parameter. Done.
   // There is something vaguely distressing about returning to the beginning of a
   // function, but there's nothing wrong with it.
   void* initialStack[] = {0, 0, 0, 0, Microthread::threadFunction, Microthread::threadReturn, this};
   m_stack.insert(m_stack.begin(), (StackType*)initialStack, ((StackType*)initialStack) + sizeof(initialStack));
   return true;
}

void __cdecl Microthread::threadFunction(void *pData)
{
   static_cast<Microthread*>(pData)->thread_proc();
   static_cast<Microthread*>(pData)->markAsDead();
}

void __cdecl Microthread::threadReturn()
{
   // We either need to loop forever here or we need to tell the scheduler
   // to not schedule dead threads. It is wasteful to check before scheduling
   // each thread whether it is dead, since this will also need to be checked
   // at a higher level (to see if it should be deleted), so I choose to loop
   // endlessly. If you remove your dead threads promptly, then this is the
   // best choice.
   while (true)
   {
      s_activeThread->wait_one_frame();
   }
   //DEBUG_BREAK();  // We should never get here.
}

void __declspec(naked) Microthread::microThreadSleep()
{
   // Six registers need to be preserved and restored, and this routine
   // is twelve instructions long. Therefore, without using multi-register
   // move instructions, this routine is probably as short as it can be.
   // We could use the multi-register push/pop instructions pushad and popad
   // but that would mean pushing and popping three extra registers. I'm
   // not sure if that would be faster.

   __asm
   {
      // Preserve all of the registers that VC++ insists we preserve.
      // VC++ does not care if we preserve eax, ecx and edx.
      push  ebx
         push  ebp
         push  esi
         push  edi

         // Swap esp and m_globalStackPointer
         mov      eax, esp
         mov      esp, m_globalStackPointer
         mov      m_globalStackPointer, eax

         // Restore all of the registers that we previously preserved.
         // Yes, they're coming off of a different stack - they were
         // carefully placed there earlier.
         pop      edi
         pop      esi
         pop      ebp
         pop      ebx
         ret
   }
}

bool Microthread::update()
{   
   // Dead threads will be scheduled, but they won't do anything.
   if (m_dead)  // Don't schedule dead threads.
      return false;
   // This is very bad. Don't schedule one thread from another.


   // The RestoreStack function assumes that it can use the
   // s_scriptStack area of memory, but the currently executing
   // thread is already using it. It is possible to orchestrate a switch,
   // but I have chosen not too.

   s_activeThread = this;
   restoreStack(m_stack);
   // This function is pure magic. It switches to the thread's stack, and implicitly
   // to the thread, and when it calls MicroThreadSleep() it switches right back to
   // here.
   microThreadSleep();
   preserveStack(m_stack);
   s_activeThread = NULL;
   return !m_dead;
}

void Microthread::preserveStack(std::vector<StackType>& StackStorage)
{
   size_t StackSize = s_stack_top - m_globalStackPointer;
   // For some reason a resize and then memmove is much faster than a
   // clear and then insert.
   StackStorage.resize(StackSize);
   memmove(&StackStorage[0], m_globalStackPointer, StackSize);
}

void Microthread::restoreStack(std::vector<StackType>& StackStorage)
{
   m_globalStackPointer = s_stack_top - StackStorage.size();
   memmove(m_globalStackPointer, &(StackStorage[0]), StackStorage.size() * sizeof(StackType));
}

void Microthread::wait_one_frame()
{
   microThreadSleep(); //we should pass the pointer to the stack but it dosent work
}


