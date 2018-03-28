// GraphEngine.Jit.Native.cpp : Defines the exported functions for the DLL application.
//

#include <asmjit.h>
#include <TrinityCommon.h>

using namespace asmjit;

JitRuntime g_jitruntime;

typedef int(*Func)(void);

DLL_EXPORT TrinityErrorCode test(OUT Func& fn)
{
    CodeHolder code;                        // Holds code and relocation information.
    code.init(g_jitruntime.getCodeInfo());  // Initialize to the same arch as JIT runtime.

    X86Assembler a(&code);                  // Create and attach X86Assembler to `code`.
    a.mov(x86::eax, 1);                     // Move one to 'eax' register.
    a.ret();                                // Return from function.

    Error err = g_jitruntime.add(&fn, &code);
    return err ? TrinityErrorCode::E_FAILURE
        : TrinityErrorCode::E_SUCCESS;
}

DLL_EXPORT int test2(Func fn)
{
    int x = fn();
    return x;
}

DLL_EXPORT JitRuntime* NewJitRuntime()
{
    return new JitRuntime();
}

DLL_EXPORT void DeleteJitRuntime(JitRuntime* rt)
{
    delete rt;
}

DLL_EXPORT CodeHolder* NewCodeHolder(JitRuntime* rt)
{
    auto holder = new CodeHolder();
    holder->init(rt->getCodeInfo());
    return holder;
}

DLL_EXPORT void DeleteCodeHolder(CodeHolder* code)
{
    delete code;
}

DLL_EXPORT X86Assembler* NewX86Assembler(CodeHolder* code)
{
    auto a = new X86Assembler(code);
    return a;
}

DLL_EXPORT void* AddFunction(JitRuntime* rt, CodeHolder* code)
{
    void* fn = nullptr;
    if (rt->add(&fn, code)) throw "AddFunction";
    return fn;
}

DLL_EXPORT void RemoveFunction(JitRuntime* rt, void* fn)
{
    if (rt->release(fn)) throw "RemoveFunction";
}