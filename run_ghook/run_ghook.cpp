#include"../ghook/include/phnt_windows.h"
#include"../ghook/include/phnt.h"
#include<iostream>
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"ntdll.lib")
using namespace std;
int main()
{
#ifdef _DEBUG
#ifdef _X86_
    LoadLibrary(L"../Debug/ghook.dll");
#else
    LoadLibrary(L"../x64/Debug/ghook64.dll");
#endif
#else
#ifdef _X64_
    auto h = LoadLibrary(L"../x64/Release/ghook64.dll");
#else
    LoadLibrary(L"../Release/ghook.dll");
#endif
#endif

    //API hook 是否成功 测试
    
    //InternetConnectW(0, 0, 0, 0, 0, 0, 0, 0);

    //NtCreateFile(0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0);
   
	WCHAR temp[MAX_PATH]{};
	GetTempPathW(MAX_PATH, temp);

	OBJECT_ATTRIBUTES oa;
	HANDLE fileHandle = NULL;
	NTSTATUS status = NULL;
	UNICODE_STRING fileName;
	IO_STATUS_BLOCK osb;

	wstring wstemp(temp);
	wstemp.append(L"testsbb.txt");
	wstemp.insert(0, L"\\??\\");

	RtlInitUnicodeString(&fileName, wstemp.c_str());

	ZeroMemory(&osb, sizeof(IO_STATUS_BLOCK));
	InitializeObjectAttributes(&oa, &fileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	NTSTATUS Status = NtCreateFile(
		&fileHandle,
		FILE_GENERIC_WRITE,
		&oa,
		&osb,
		0,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_WRITE,
		FILE_OVERWRITE_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

    //cout << "helooooooooooo\n";


	printf("[-] Status %x\n", Status);







    system("pause");
    return 0;
}
