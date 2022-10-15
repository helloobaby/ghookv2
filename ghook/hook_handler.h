#include "minictx.h"

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
		POBJECT_ATTRIBUTES r8 = (POBJECT_ATTRIBUTES)context->R8;
		logger->info(std::format(L"[-] NtCreateFile path {}\n", r8->ObjectName->Buffer));
	}if (context->OriFunctionPointer == OriNtAllocateVirtualMemory) {
		size_t* r9 = (size_t*)context->R9;
		logger->info(std::format(L"[-] NtAllocateVirtualMemory addr {:#x}\n",*r9));
	}
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;