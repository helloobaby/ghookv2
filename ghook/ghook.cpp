#include <cstdio>
#include <format>
#include <windows.h>
#include <wininet.h>

#include "ghook.h"
#include "MinHook.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "hook_handler.h"
#include "minictx.h"

extern wchar_t exepath[512];
extern std::shared_ptr<spdlog::logger> logger;

PVOID OriNtCreateFile;
PVOID NtCreateFileShellcode;

PVOID OriNtAllocateVirtualMemory;
PVOID NtAllocateVirtualMemoryShellcode;

static int cmpfunc(const void* a, const void* b)
{
	return (*(char*)a - *(char*)b);
}

//为被hook的api函数创建一个shellcode,跳转到我们的handler
PVOID gen_shellcode() {
	static constexpr uint64_t PAGE_SIZE = 0x1000;

	PVOID shellcode_base = nullptr;
	shellcode_base = VirtualAlloc(NULL, PAGE_SIZE, MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	if (!shellcode_base) {	//分配内存失败
		return nullptr;
	}

	
	//api monitor是把IAT中的指针

	// shellcode需要建立minictx,保存当时的环境,然后这个环境作为参数传给hook_handler
	static char shellcode[] = {

		// 保存环境
		0x54,									//push rsp
		0x41, 0x57,								//push r15				
		0x41, 0x56,								//push r14
		0x41, 0x55,								//push r13
		0x41, 0x54,								//push r12
		0x41, 0x53,								//push r11
		0x41, 0x52,								//push r10
		0x41, 0x51,								//push r9
		0x41, 0x50,								//push r8
		0x57,									//push rdi 
		0x56,									//push rsi
		0x55,									//push rbp
		0x53,									//push rbx
		0x52,									//push rdx
		0x51,									//push rcx
		0x50,									//push rax

		// 直接call一个地址会有4gb地址空间限制
		0x48,0xb9,00,00,00,00,00,00,00,00,      //mov rcx,原始函数地址(利用fill_shellcode_ori_function填入)
		0x51,									//push rcx ,把这个函数指针放在栈上
		0x48, 0x89, 0xE1,						//mov rcx,rsp

		// ff 15 后面4字节是相对rip的偏移(shellcode最后8个字节放这个call的地址)
		0xff,0x15,30,00,00,00,					//call qword ptr ds:[A]跳转到hook_handler_asm函数地址

		// 恢复环境
		0x41,0x5f,								//pop r15   r15指向原函数指针(这里要污染一个r15寄存器)
		0x58,									//pop rax
		0x59,									//pop rcx
		0x5A,									//pop rdx
		0x5B,									//pop rbx
		0x5D,									//pop rbp
		0x5E,									//pop rsi
		0x5F,									//pop rdi
		0x41, 0x58,								//pop r8
		0x41, 0x59,								//pop r9
		0x41, 0x5A,								//pop r10
		0x41, 0x5B,								//pop r11
		0x41, 0x5C,								//pop r12
		0x41, 0x5D,								//pop r13
		0x41, 0x5E,								//pop r14
		//0x41, 0x5F,							//pop r15
		0x48, 0x83, 0xC4, 0x10,					//add rsp,8 代替pop r15,pop rsp

		//调用原来函数
		0x41, 0xFF, 0xE7,						//jmp r15 只能用jmp 不能用call call会改变rsp

		00,00,00,00,00,00,00,00,				//放一个地址，就是上面的hook_handler_asm
	};

	static_assert(sizeof(shellcode) < PAGE_SIZE);

	memcpy((void*)((ULONG_PTR)shellcode + sizeof(shellcode) - 8), &hook_handler_asm_ptr, 8);

	memcpy(shellcode_base, shellcode, sizeof(shellcode));
	return shellcode_base;
}

void fill_shellcode_ori_function(PVOID shellcode, PVOID ori_function_pointer) {
	// 如果shellcode改了的话,这里的偏移要改一下
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

	OriNtCreateFile;
	NtCreateFileShellcode = gen_shellcode();
	r = MH_CreateHook(NtCreateFile, NtCreateFileShellcode, &OriNtCreateFile);	//执行api会直接跳转到shellcode
	fill_shellcode_ori_function(NtCreateFileShellcode, OriNtCreateFile);

	OriNtAllocateVirtualMemory;
	NtAllocateVirtualMemoryShellcode = gen_shellcode();
	r = MH_CreateHook(NtAllocateVirtualMemory, NtAllocateVirtualMemoryShellcode, &OriNtAllocateVirtualMemory);
	fill_shellcode_ori_function(NtAllocateVirtualMemoryShellcode, OriNtAllocateVirtualMemory);

	if (r != MH_OK) {
		return;
	}

	// 使能hook
	r = MH_EnableHook(NtCreateFile);
	if (r != MH_OK) {
		return;
	}
	r = MH_EnableHook(NtAllocateVirtualMemory);
	if (r != MH_OK) {
		return;
	}
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

