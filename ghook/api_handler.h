// 这个头文件有隐藏依赖的问题

extern std::shared_ptr<spdlog::logger> logger;


void handler_NtCreateFile(minictx* context) {
	POBJECT_ATTRIBUTES r8 = (POBJECT_ATTRIBUTES)context->R8;
	logger->info(std::format(L"[-] NtCreateFile path {}\n", r8->ObjectName->Buffer));
}

void handler_NtAllocateVirtualMemory(minictx* context) {
	size_t* r9 = (size_t*)context->R9;
	DWORD64 rsp = (DWORD64)context->Rsp;
	DWORD AllocType = *(ULONG64*)((ULONG64)rsp + 0x28);
	DWORD ProtectType = *(ULONG64*)((ULONG64)rsp + 0x30);
	logger->info(std::format(L"[-] NtAllocateVirtualMemory size {:#x}\n", *r9));
	logger->info(std::format(L"[-] NtAllocateVirtualMemory AlloctionType {:#x}\n", AllocType));
	logger->info(std::format(L"[-] NtAllocateVirtualMemory ProtectType {:#x}\n", ProtectType));
}