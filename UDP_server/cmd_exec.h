#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int SystemCapture(
	std::string  Command,
	std::string  RunDir,
	std::string& ListStdOut,
	uint32_t& RetCode)
{
	SECURITY_ATTRIBUTES  security_attributes;
	HANDLE stdout_rd = INVALID_HANDLE_VALUE;
	HANDLE stdout_wr = INVALID_HANDLE_VALUE;

	security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_attributes.bInheritHandle = TRUE;
	security_attributes.lpSecurityDescriptor = nullptr;

	if (!CreatePipe(&stdout_rd, &stdout_wr, &security_attributes, 0) ||
		!SetHandleInformation(stdout_rd, HANDLE_FLAG_INHERIT, 0)) {
		return -1;
	}

	PROCESS_INFORMATION  process_info;
	STARTUPINFOA startup_info;
	ZeroMemory(&startup_info, sizeof(STARTUPINFO));
	startup_info.cb = sizeof(STARTUPINFO);
	startup_info.hStdInput = 0;
	startup_info.hStdOutput = stdout_wr;
	startup_info.hStdError = stdout_wr;
	startup_info.dwFlags |= STARTF_USESTDHANDLES;

	std::string CmdLineStr = "C:\\Windows\\System32\\cmd.exe /c ";
	CmdLineStr.append(Command);
	CmdLineStr.push_back(0);
	int Success = CreateProcessA(
		nullptr,
		CmdLineStr.data(),
		nullptr,
		nullptr,
		TRUE,
		0,
		nullptr,
		RunDir.c_str(),
		&startup_info,
		&process_info
	);
	CloseHandle(stdout_wr);

	if (!Success) {
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
		CloseHandle(stdout_rd);
		return -4;
	}
	else {
		CloseHandle(process_info.hThread);
	}

	thread  stdout_thread;
	if (stdout_rd) {
		stdout_thread = thread([&stdout_rd, &ListStdOut]() {
			DWORD  n;
			const size_t bufsize = 40;
			char        buffer[bufsize];
			while (true)
			{
				n = 0;
				int Success = ReadFile(
					stdout_rd,
					buffer,
					(DWORD)bufsize,
					&n,
					nullptr
				);
				if (!Success || n == 0)
					break;
				ListStdOut.append(std::string{ buffer, n });
			}
			});
	}

	WaitForSingleObject(process_info.hProcess, INFINITE);
	if (!GetExitCodeProcess(process_info.hProcess, (DWORD*)&RetCode))
		RetCode = -1;

	CloseHandle(process_info.hProcess);

	if (stdout_thread.joinable())
		stdout_thread.join();

	CloseHandle(stdout_rd);

	return 0;
}