#include "ScriptFunction.hpp"
#include "Memory.hpp"
#include "rage/scrProgram.hpp"
#include "rage/scrThread.hpp"
#include "rage/scrValue.hpp"

rage::scrThreadState (*g_RunScriptThread)(rage::scrValue* stack, rage::scrValue** globals, rage::scrProgram* program, rage::scrThreadContext* context) = nullptr;
rage::scrValue** g_ScriptGlobals = nullptr;

void ScriptFunction::CallImpl(rage::scrThread* thread, rage::scrProgram* program, uint32_t pc, const std::vector<uint64_t>& args, void* returnValue, uint32_t returnSize)
{
    static bool init = [] {
        if (g_IsEnhanced)
        {
            if (auto addr = Memory::ScanPattern("49 63 41 1C"))
                g_RunScriptThread = addr->Sub(0x24).As<decltype(g_RunScriptThread)>();

            if (auto addr = Memory::ScanPattern("48 8B 8E B8 00 00 00 48 8D 15 ? ? ? ? 49 89 D8"))
                g_ScriptGlobals = addr->Add(7).Add(3).Rip().As<decltype(g_ScriptGlobals)>();
        }
        else
        {
            if (auto addr = Memory::ScanPattern("E8 ? ? ? ? 48 85 FF 48 89 1D"))
                g_RunScriptThread = addr->Add(1).Rip().As<decltype(g_RunScriptThread)>();

            if (auto addr = Memory::ScanPattern("48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF 48 89 1D"))
                g_ScriptGlobals = addr->Add(3).Rip().As<decltype(g_ScriptGlobals)>();
        }

        return true;
    }();

    if (!g_RunScriptThread || !g_ScriptGlobals)
        return;

    if (!thread || !program || !pc)
        return;

    auto& curThread = rage::scrThread::GetCurrentThread(); // don't use a copy here
    auto ogThread = curThread;
    auto ogState = *thread->Context()->State();

    curThread = thread;
    rage::scrThread::SetCurrentThreadActive(true);

    alignas(4) uint8_t ctxStorage[0xB0]; // we must use a copy
    std::memcpy(ctxStorage, thread->Context(), g_IsEnhanced ? 0xB0 : 0xA8);

    auto ctx = reinterpret_cast<rage::scrThreadContext*>(ctxStorage);
    auto stack = thread->Stack();
    auto topStack = *ctx->StackPointer();

    for (auto& arg : args)
        stack[(*ctx->StackPointer())++].Any = arg;

    stack[(*ctx->StackPointer())++].Any = 0;
    *ctx->ProgramCounter() = pc;
    *ctx->State() = rage::scrThreadState::RUNNING;

    while (g_RunScriptThread(stack, g_ScriptGlobals, program, ctx) == rage::scrThreadState::IDLE)
        WAIT(static_cast<DWORD>(*ctx->WaitTimer() * 1000.0f)); // for yielding functions (e.g., calling WAIT)

    curThread = ogThread;
    rage::scrThread::SetCurrentThreadActive(ogThread != nullptr);

    // If the called script function uses a START_NEW_SCRIPT_* native,
    // StartNewScriptInternal sets the current thread state to RUNNING.
    // Since we spoof the current thread, this ends up modifying our target
    // thread's state. This is a problem when the thread was meant to stay
    // paused while we call specific functions in it, so we restore the original
    // state if it was changed during the call. KILLED is not restored on purpose,
    // if the script decided to terminate itself (e.g. via cleanup), we let it die.
    if (*thread->Context()->State() != ogState && *thread->Context()->State() != rage::scrThreadState::KILLED)
        *thread->Context()->State() = ogState;

    if (returnValue)
        std::memcpy(returnValue, stack + topStack, returnSize);
}