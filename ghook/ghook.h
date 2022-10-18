#pragma once
#pragma warning(disable:4005)
#include"include/phnt_windows.h"
#include"include/phnt.h"

void OutputDebug(const WCHAR* strOutputString, ...);
void MainWork();

extern "C"
{
__declspec(dllexport) void __stdcall export_thunk();
}


class Hooker {
 public:
	 // 提供需要被hook的函数地址
  bool Hook(PVOID function_address);

  PVOID GenShellCode();
  
  void FillShellCode(PVOID shellcode, PVOID ori_function_pointer,PVOID hooked_function);

  PVOID GetHookedFunction() { return hooked_function_; }

  static bool IsLibraryInit() { return minhook_init_; }
  static bool LibraryInit();

 private:
  static inline bool minhook_init_ = 0;		//c++17

  // 被hook的函数起始地址
  PVOID hooked_function_;
  // 通过这个指针调用原来的函数
  PVOID ori_pointer_;				
  // 给每个被hook的函数分配的shellcode
  PVOID shellcode_;
  // shellcode的大小
  unsigned int shellcode_size_;
};


