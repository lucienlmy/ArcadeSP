#pragma once

namespace rage
{
    class scrThread;
    class scrProgram;
}

struct ScriptFunction
{
private:
    template <typename Arg>
    static void PushArg(uint64_t* stack, uint32_t& stackPtr, Arg&& value)
    {
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<Arg>>*>(reinterpret_cast<uint64_t*>(stack) + (stackPtr++)) = std::forward<Arg>(value);
    }

    static void CallImpl(rage::scrThread* thread, rage::scrProgram* program, uint32_t pc, const std::vector<uint64_t>& args, void* returnValue = 0, uint32_t returnSize = 0);

public:
    template <typename Ret = void, typename... Args>
    static Ret Call(rage::scrThread* thread, rage::scrProgram* program, uint32_t pc, Args... args)
    {
        uint32_t index{};
        std::vector<uint64_t> params(sizeof...(Args));
        (PushArg(params.data(), index, std::forward<Args>(args)), ...);
        if constexpr (!std::is_same_v<Ret, void>)
        {
            Ret returnValue;
            CallImpl(thread, program, pc, params, &returnValue, sizeof(returnValue));
            return returnValue;
        }
        else
        {
            CallImpl(thread, program, pc, params);
        }
    }
};