#include "minictx.h"
#include "api_handler.h"

#include <iostream>
#include <cassert>
#include <mutex>


extern "C" void hook_handler_asm();

extern std::shared_ptr<spdlog::logger> logger;

// �������������̰߳�ȫ
extern "C" void hook_handler_c(minictx * context) {

    assert(context);
    //logger->info(L"hook_handler_c enter");		//DEBUG

    // hook NtAllocateVirtualMemory ������
    if (context->HookedFunction == &NtAllocateVirtualMemory) {
      handler_NtAllocateVirtualMemory(context);
    } else if (context->HookedFunction == &BitBlt) {
      handler_BitBlt(context); 
    }
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;