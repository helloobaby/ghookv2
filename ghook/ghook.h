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
	 // �ṩ��Ҫ��hook�ĺ�����ַ
  bool Hook(PVOID function_address);

  PVOID GenShellCode();
  
  void FillShellCode(PVOID shellcode, PVOID ori_function_pointer,PVOID hooked_function);

  PVOID GetHookedFunction() { return hooked_function_; }

  static bool IsLibraryInit() { return minhook_init_; }
  static bool LibraryInit();

 private:
  static inline bool minhook_init_ = 0;		//c++17

  // ��hook�ĺ�����ʼ��ַ
  PVOID hooked_function_;
  // ͨ�����ָ�����ԭ���ĺ���
  PVOID ori_pointer_;				
  // ��ÿ����hook�ĺ��������shellcode
  PVOID shellcode_;
  // shellcode�Ĵ�С
  unsigned int shellcode_size_;
};


