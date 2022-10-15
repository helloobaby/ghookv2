#include "minictx.h"

#include <iostream>
#include <cassert>
#include <mutex>

extern "C" void hook_handler_asm();

std::mutex m1;
// 这个函数必须多线程安全
extern "C" void hook_handler_c(minictx * context) {
	assert(context);

	std::lock_guard<std::mutex> lg(m1);

	printf("[+]hook_handler \n");
	printf("[+]ori function pointer %p\n", context->OriFunctionPointer);

	if (context->OriFunctionPointer == NtCreateFile) {
		POBJECT_ATTRIBUTES r8 = (POBJECT_ATTRIBUTES)context->R8;
		printf("%ws\n", r8->ObjectName->Buffer);			//unsafe
	}
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;