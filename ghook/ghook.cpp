#include <cstdio>
#include <format>
#include <windows.h>
#include <wininet.h>

#include "ghook.h"
#include "MinHook.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "hook_handler.h"

extern wchar_t exepath[512];
extern std::shared_ptr<spdlog::logger> logger;

enum function_hooked_type{
	NtCreateFileType,
};


#ifdef _DEBUG
#ifdef _X86_
#pragma comment(lib,"libMinHook.x86D.lib")
#else
#pragma comment(lib,"libMinHook.x64D.lib")
#endif
#else
#ifdef _X64_
#pragma comment(lib,"libMinHook.x64R.lib")
#else
#pragma comment(lib,"libMinHook.x86R.lib")
#endif
#endif

static int cmpfunc(const void* a, const void* b)
{
	return (*(char*)a - *(char*)b);
}

//为函数创建一个shellcode,跳转到我们的handler
PVOID gen_shellcode() {
	static constexpr uint64_t PAGE_SIZE = 0x1000;

	PVOID shellcode_base = nullptr;
	shellcode_base = VirtualAlloc(NULL, PAGE_SIZE, MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	if (!shellcode_base) {	//分配内存失败
		return nullptr;
	}


	//api monitor是把IAT中的指针
	static char shellcode[] = {
	0x4c,0x89,0x4c,0x24,0x20,		//mov qword ptr ss:[rsp+20],r9
	0x4c,0x89,0x44,0x24,0x18,       //mov qword ptr ss:[rsp+18],r8
	0x48,0x89,0x54,0x24,0x10,       //mov qword ptr ss:[rsp+10],rdx
	0x48,0x89,0x4c,0x24,0x08,		//mov qword ptr ss:[rsp+8],rcx
	0x48,0x83,0xec,0x28,		    //sub rsp,0x28
	0x49,0xbf,00,00,00,00,00,00,00,00, //mov r15,ori_function_pointer

	0xff,0x15,05,00,00,00,			//call qword ptr ds:[A]	 跳转到hook_handler_asm函数地址
	0x48,0x83,0xC4,0x28,			//add rsp,0x28
	0xc3,							//ret
	00,00,00,00,00,00,00,00,		//放一个地址，就是上面的hook_handler_asm
	};

	static_assert(sizeof(shellcode) < PAGE_SIZE);

	memcpy((void*)((ULONG_PTR)shellcode + sizeof(shellcode) - 8), &hook_handler_asm_ptr, 8);

	memcpy(shellcode_base, shellcode, sizeof(shellcode));
	return shellcode_base;
}

void fill_shellcode_ori_function(PVOID shellcode, PVOID ori_function_pointer) {
	memcpy((void*)((ULONG_PTR)shellcode + 26), &ori_function_pointer, sizeof(void*));
}

void MainWork()
{
	auto r = MH_Initialize();

	if (r != MH_OK)
	{
		logger->info(L"minhook init failed");
		return;
	}
	
	void* pfnNtCreateFile = GetProcAddress(LoadLibraryW(L"ntdll.dll"), "NtCreateFile");
	if (!pfnNtCreateFile)
	{
		logger->info(L"NtCreateFile not found");
		return;
	}

	PVOID OriNtCreateFile;
	PVOID NtCreateFileShellcode = gen_shellcode();
	MH_CreateHook(pfnNtCreateFile, NtCreateFileShellcode, &OriNtCreateFile);
	fill_shellcode_ori_function(NtCreateFileShellcode, OriNtCreateFile);

	// 使能hook
	MH_EnableHook(pfnNtCreateFile);

}


void OutputDebug(const WCHAR* strOutputString, ...) {
	WCHAR strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	//in stdio.h
	_vsnwprintf_s(strBuffer, ARRAYSIZE(strBuffer) - 1, ARRAYSIZE(strBuffer) - 1, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(strBuffer);
}

//
// 主要就是弹出一个框 指示hook成功了
//
void TestApiHookSuccess(const WCHAR* strOutputString) {
	MessageBoxW(NULL, strOutputString, L"ghook", MB_OK);
}


void __stdcall export_thunk()
{
	OutputDebug(L"[ghook]export_thunk\n");
}

