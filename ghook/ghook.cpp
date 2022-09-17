#include <cstdio>
#include <format>
#include <windows.h>
#include <wininet.h>

#include "ghook.h"
#include "MinHook.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

extern wchar_t exepath[512];
extern std::shared_ptr<spdlog::logger> logger;
#include "NtCreateFile.h"
#include "InternetConnectW.h"


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

void MainWork()
{
	auto r = MH_Initialize();

	if (r != MH_OK)
	{
		logger->info(L"[ghook]minhook init failed");
		return;
	}
	
	void* pfnNtCreateFile = GetProcAddress(LoadLibraryW(L"ntdll.dll"), "NtCreateFile");
	if (!pfnNtCreateFile)
	{
		logger->info(L"[ghook]NtCreateFile not found");
		return;
	}

	if (MH_CreateHook(
		pfnNtCreateFile, 
		DetourNtCreateFile, 
		(LPVOID*)&OriNtCreateFile) != MH_OK) {

		logger->info(L"[ghook]hook NtCreateFile failed");
		return;
	}

	void* pfnInternetConnectW = GetProcAddress(LoadLibraryW(L"Wininet.dll"), "InternetConnectW");
	if (!pfnInternetConnectW) {
		logger->info(L"[ghook]InternetConnectA not found");
		return;
	}

	if (MH_CreateHook(pfnInternetConnectW, DetourInternetConnectW, (LPVOID*)&OriInternetConnectW) != MH_OK) {
		logger->info(L"[ghook]hook InternetConnectW");
		return;
	}



	//最后再使能所有hook
	MH_EnableHook(pfnNtCreateFile);
	MH_EnableHook(pfnInternetConnectW);

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

