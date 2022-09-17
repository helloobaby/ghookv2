#pragma once

decltype(auto) OriInternetConnectW = (&InternetConnectW);

extern "C" {
	HINTERNET NTAPI DetourInternetConnectW(
		HINTERNET     hInternet,
		LPCWSTR       lpszServerName,
		INTERNET_PORT nServerPort,
		LPCWSTR       lpszUserName,
		LPCWSTR       lpszPassword,
		DWORD         dwService,
		DWORD         dwFlags,
		DWORD_PTR     dwContext
	);
}

HINTERNET NTAPI DetourInternetConnectW(
	HINTERNET     hInternet,
	LPCWSTR       lpszServerName,
	INTERNET_PORT nServerPort,
	LPCWSTR       lpszUserName,
	LPCWSTR       lpszPassword,
	DWORD         dwService,
	DWORD         dwFlags,
	DWORD_PTR     dwContext
){
	//TestApiHookSuccess(L"DetourInternetConnectW called\n");

	HINTERNET return_value;

	if(lpszServerName)
	{
		logger->info(std::format(L"[{}][InternetConnectW][ServerName : {}   Port :{}]",exepath ,lpszServerName, nServerPort));
	}



	return_value = OriInternetConnectW(hInternet, lpszServerName, nServerPort, 
		lpszUserName, lpszPassword, dwService,dwFlags, dwContext);




	return return_value;
}