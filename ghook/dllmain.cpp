// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "ghook.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <shlobj.h>

#pragma comment(lib,"ntdll")
#pragma comment(lib,"wininet.lib")

wchar_t exepath[512]{};
char desktop_path[512]{};                           //用户名不要有中文

std::shared_ptr<spdlog::logger> logger;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        GetModuleFileNameW(NULL,exepath,512);

        spdlog::flush_on(spdlog::level::info);      //设置优先级 >= info的信息立刻写入, 因为我们不需要性能

        SHGetSpecialFolderPathA(0, desktop_path, CSIDL_DESKTOPDIRECTORY, 0);
        //在桌面创建一个日志文件
        logger = spdlog::basic_logger_mt("log2", std::string(desktop_path).append("\\ghook_log.txt"), false);

        logger->info(std::format(L"[ghook]dll loaded : {}",exepath));

        MainWork(); //ghook.cpp
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

