#include "minictx.h"
#include "api_handler.h"

#include <iostream>
#include <cassert>
#include <mutex>


extern "C" void hook_handler_asm();

extern std::shared_ptr<spdlog::logger> logger;

extern PVOID OriNtCreateFile;
extern PVOID OriNtAllocateVirtualMemory;

// 这个函数必须多线程安全
extern "C" void hook_handler_c(minictx * context) {
	assert(context);

	//printf("[+]hook_handler \n");
	//printf("[+]ori function pointer %p\n", context->OriFunctionPointer);

	if (context->OriFunctionPointer == OriNtCreateFile) {
		handler_NtCreateFile(context);
	}if (context->OriFunctionPointer == OriNtAllocateVirtualMemory) {
		handler_NtAllocateVirtualMemory(context);
	}
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;