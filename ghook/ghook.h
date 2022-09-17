#pragma once
#pragma warning(disable:4005)
#include"include/phnt_windows.h"
#include"include/phnt.h"

void OutputDebug(const WCHAR* strOutputString, ...);
void TestApiHookSuccess(const WCHAR* strOutputString);

void MainWork();

extern "C"
{
	__declspec(dllexport) void __stdcall export_thunk();
}



