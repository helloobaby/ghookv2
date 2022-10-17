#include "minictx.h"
#include "api_handler.h"

#include <iostream>
#include <cassert>
#include <mutex>


extern "C" void hook_handler_asm();

extern std::shared_ptr<spdlog::logger> logger;

// 这个函数必须多线程安全
extern "C" void hook_handler_c(minictx * context) {

    assert(context);
    //logger->info(L"hook_handler_c enter");		//DEBUG

    // hook NtAllocateVirtualMemory 会死锁
    if (context->HookedFunction == &NtAllocateVirtualMemory) {
      handler_NtAllocateVirtualMemory(context);
    } else if (context->HookedFunction == &BitBlt) {
      handler_BitBlt(context); 
    }
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;