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

#pragma warning(disable : 4838)
#pragma warning(disable : 4309)

extern wchar_t exepath[512];
extern std::shared_ptr<spdlog::logger> logger;

// Ϊ��hook��api��������һ��shellcode,��ת�����ǵ�handler
PVOID Hooker::GenShellCode() {
	static constexpr uint64_t PAGE_SIZE = 0x1000;

	PVOID shellcode_base = nullptr;
	shellcode_base = VirtualAlloc(NULL, PAGE_SIZE, MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	if (!shellcode_base) {	//�����ڴ�ʧ��
		return nullptr;
	}

	// shellcode��Ҫ����minictx,���浱ʱ�Ļ���,Ȼ�����������Ϊ��������hook_handler
	static char shellcode[] = {

		// ���滷��
		0x54,									//push rsp	1
		0x41, 0x57,								//push r15	2			
		0x41, 0x56,								//push r14  3
		0x41, 0x55,								//push r13  4
		0x41, 0x54,								//push r12  5
		0x41, 0x53,								//push r11  6
		0x41, 0x52,								//push r10  7
		0x41, 0x51,								//push r9   8
		0x41, 0x50,								//push r8   9
		0x57,									//push rdi  10
		0x56,									//push rsi  11
		0x55,									//push rbp  12 
		0x53,									//push rbx  13
		0x52,									//push rdx  14
		0x51,									//push rcx  15
		0x50,									//push rax  16

        0x48, 0xb9, /*@1*/ 00, 00, 00, 00, 00, 00, 00,00,                //mov rcx,hooked_function
		0x51,									//push rcx  17
		0x48, 0x89, 0xE1,						//mov rcx,rsp

		// ff 15 ����4�ֽ������rip��ƫ��(shellcode���8���ֽڷ����call�ĵ�ַ)
		0xff,0x15,35,00,00,00,					//call qword ptr ds:[A]��ת��hook_handler_asm������ַ

		// �ָ�����
		0x41,0x5f,								//pop struct minictx.HookedFunction����
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
		0x41, 0x5F,							    //pop r15
		0x48, 0x83, 0xC4, 0x8,					//add rsp,8h	//�൱�ڰ�rsp������

		//jmp ����ԭ������ (call�����ٿ���)
		0xff, 0x25,8,00,00,00,						




		00,00,00,00,00,00,00,00,				//��һ����ַ�����������hook_handler_asm
		00,00,00,00,00,00,00,00,				//��ori pointer ,����ԭ���ĺ�����
	};

	static_assert(sizeof(shellcode) < PAGE_SIZE);
    shellcode_size_ = sizeof(shellcode);
	memcpy((void*)((ULONG_PTR)shellcode + sizeof(shellcode) - 16), &hook_handler_asm_ptr, 8);

	memcpy(shellcode_base, shellcode, sizeof(shellcode));
	return shellcode_base;
}

void Hooker::FillShellCode(PVOID shellcode, PVOID ori_function_pointer,PVOID hooked_function) {
	// ���shellcode���˵Ļ�,�����ƫ��Ҫ��һ��
  memcpy((void*)((ULONG_PTR)shellcode + shellcode_size_ - 8),
         &ori_function_pointer, sizeof(void*));

  //@1��
    memcpy((void*)((ULONG_PTR)shellcode + 26), &hooked_function, sizeof(void*));
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


// ����һ������ ��ʱ������
void __stdcall export_thunk()
{
	OutputDebug(L"[ghook]export_thunk\n");
}

bool Hooker::LibraryInit() {
  minhook_init_ = true;
  return MH_Initialize() == MH_OK;
}

bool Hooker::Hook(PVOID function_address) {
  if (!IsLibraryInit())
    LibraryInit();

  MH_STATUS r;
  shellcode_ = GenShellCode();
  hooked_function_ = function_address;
  r = MH_CreateHook(function_address, shellcode_, &ori_pointer_);
  if (r != MH_OK)
    return false;

  // ע�ⲻ��&ori_pointer_
  FillShellCode(shellcode_, ori_pointer_,hooked_function_);

  return MH_EnableHook(function_address) == MH_OK;
}

void MainWork() {
  bool success = true;

  //Hooker NtAllocateVirtualMemoryHooker;
  //success = NtAllocateVirtualMemoryHooker.Hook(&::NtAllocateVirtualMemory);

  //Hooker BitBltHooker;
  //success &= BitBltHooker.Hook(&::BitBlt);

  Hooker NtCreateFileHooker;
  success = NtCreateFileHooker.Hook(&NtCreateFile);

    if (!success) {
    logger->info(L"hook something failed");
    return;
  }
}