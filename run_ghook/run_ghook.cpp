#include<windows.h>
#include<winInet.h>
#include<iostream>
#pragma comment(lib,"wininet.lib")
using namespace std;
int main()
{
#ifdef _DEBUG
#ifdef _X86_
    LoadLibrary(L"../Debug/ghook.dll");
#else
    LoadLibrary(L"../x64/Debug/ghook.dll");
#endif
#else
#ifdef _X64_
    auto h = LoadLibrary(L"../x64/Release/ghook.dll");
#else
    LoadLibrary(L"../Release/ghook.dll");
#endif
#endif

    //API hook 是否成功 测试
    InternetConnectW(0, 0, 0, 0, 0, 0, 0, 0);















    system("pause");
    return 0;
}
