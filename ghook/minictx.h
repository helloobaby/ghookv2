#ifndef _MINI_CTX
#define _MINI_CTX
#include <windows.h>
// 可能只需要保存这些寄存器(目前只支持x64)
struct minictx {
    PVOID HookedFunction;
    PVOID OriFunctionPointer;
    DWORD64 Rax;
    DWORD64 Rcx;
    DWORD64 Rdx;
    DWORD64 Rbx;
    //DWORD64 Rsp;
    DWORD64 Rbp;
    DWORD64 Rsi;
    DWORD64 Rdi;
    DWORD64 R8;
    DWORD64 R9;
    DWORD64 R10;
    DWORD64 R11;
    DWORD64 R12;
    DWORD64 R13;
    DWORD64 R14;
    DWORD64 R15;
    DWORD64 Rsp;
};


#endif