#include "scrThread.hpp"
#include "Memory.hpp"
#include "atArray.hpp"

namespace rage
{
    scrThread*& scrThread::GetCurrentThread()
    {
        return *reinterpret_cast<scrThread**>(*reinterpret_cast<uintptr_t*>(__readgsqword(0x58)) + (g_IsEnhanced ? 0x7A0 : 0x2A50));
    }

    void scrThread::SetCurrentThreadActive(bool active)
    {
        *reinterpret_cast<bool*>(*reinterpret_cast<uintptr_t*>(__readgsqword(0x58)) + (g_IsEnhanced ? 0x7A8 : 0x2A58)) = active;
    }

    scrThread* scrThread::GetThread(uint32_t hash)
    {
        static bool init = [] {
            if (g_IsEnhanced)
            {
                if (auto addr = Memory::ScanPattern("48 8B 05 ? ? ? ? 48 89 34 F8 48 FF C7 48 39 FB 75 97"))
                    m_Threads = addr->Add(3).Rip().As<decltype(m_Threads)>();
            }
            else
            {
                if (auto addr = Memory::ScanPattern("45 33 F6 8B E9 85 C9 B8")) // Works since b323
                    m_Threads = addr->Sub(4).Rip().Sub(8).As<decltype(m_Threads)>();
            }

            return true;
        }();

        if (!m_Threads)
            return nullptr;

        for (auto& thread : *m_Threads)
        {
            if (thread && *thread->Context()->Id() && *thread->ScriptHash() == hash)
                return thread;
        }

        return nullptr;
    }
}