#include "minictx.h"

#include <iostream>

extern "C" void hook_handler_asm();


extern "C" void hook_handler_c(minictx* context) {
	printf("[+]hook_handler \n");
	printf("[+]ori function pointer %p\n", context->OriFunctionPointer);
	POBJECT_ATTRIBUTES r8 = (POBJECT_ATTRIBUTES)context->R8;
	std::wcout << r8->ObjectName->Buffer << std::endl;  //ֱ�����Buffer����ȫ,ֻ������
}


PVOID hook_handler_asm_ptr = &hook_handler_asm;