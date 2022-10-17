// 这个头文件有隐藏依赖的问题

#include "param_helper.h"
extern std::shared_ptr<spdlog::logger> logger;


void handler_NtCreateFile(minictx* context) {
	//POBJECT_ATTRIBUTES r8 = (POBJECT_ATTRIBUTES)context->R8;

	POBJECT_ATTRIBUTES r8 = get_3_param<POBJECT_ATTRIBUTES>(context);
	logger->info(std::format(L"[-] NtCreateFile path {}\n", r8->ObjectName->Buffer));
	
}

void handler_NtAllocateVirtualMemory(minictx* context) {
  size_t* r9 = get_4_param<size_t*>(context);
    DWORD AllocType = get_5_param<DWORD>(context);
    DWORD ProtectType = get_6_param<DWORD>(context);
	logger->info(std::format(L"[-] NtAllocateVirtualMemory size {:#x}\n", *r9));
	logger->info(std::format(L"[-] NtAllocateVirtualMemory AlloctionType {:#x}\n", AllocType));
	logger->info(std::format(L"[-] NtAllocateVirtualMemory ProtectType {:#x}\n", ProtectType));
}