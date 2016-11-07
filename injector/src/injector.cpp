// injector.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <Windows.h>

#include <string>

bool InjectDll(int pid, const std::string& dll) {
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
	HMODULE hKernel32 = ::GetModuleHandle(L"kernel32.dll");
	void* remoteMemoryBlock = ::VirtualAllocEx(hProcess, NULL, dll.size() + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!remoteMemoryBlock) {
		printf("remoteMemoryBlock failed\n");
		return false;
	}
	if (0 == ::WriteProcessMemory(hProcess, remoteMemoryBlock, (void*)dll.c_str(), dll.size() + 1, NULL))
	{
		printf("WriteProcessMemory failed\n");
		return false;
	}
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)::GetProcAddress(hKernel32, "LoadLibraryA"),
		remoteMemoryBlock, 0, NULL);
	if (hThread == NULL) {
		::VirtualFreeEx(hProcess, remoteMemoryBlock, dll.size(), MEM_RELEASE);
		printf("CreateRemoteThread %d", GetLastError());
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		printf("Need Pid\n");
		return 0;
	}

	long pid = atoi(argv[1]);
	std::string dllPath = argv[2];

	if (pid == 0)
	{
		printf("Need valid Pid\n");
		return 0;
	}

	InjectDll(pid, dllPath);

    return 0;
}

