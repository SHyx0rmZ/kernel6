#include <cstdint>

enum _Unwind_Reason_Code
{
    _URC_NO_REASON = 0,
    _URC_FOREIGN_EXCEPTION_CAUGHT,
    _URC_FATAL_PHASE2_ERROR,
    _URC_FATAL_PHASE1_ERROR,
    _URC_NORMAL_STOP,
    _URC_END_OF_STACK,
    _URC_HANDLER_FOUND,
    _URC_INSTALL_CONTEXT,
    _URC_CONTINUE_UNWIND
};

struct _Unwind_Exception;

typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code reason, _Unwind_Exception *ecx);
typedef int _Unwind_Action;

struct _Unwind_Exception
{
    std::uint64_t exception_class;
    _Unwind_Exception_Cleanup_Fn exception_cleanup;
    std::uint64_t reserved[2];
} __attribute__((aligned(4)));

struct _Unwind_Context
{
};

extern "C" void _Unwind_Resume(_Unwind_Exception *exception_object __attribute__((unused)))
{
}

extern "C" _Unwind_Reason_Code __gxx_personality_v0(int, _Unwind_Action, std::uint64_t, _Unwind_Exception *, _Unwind_Context *)
{
    return _URC_NO_REASON;
}
