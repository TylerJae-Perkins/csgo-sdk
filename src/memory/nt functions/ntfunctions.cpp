#include "ntfunctions.h"

//Use these as Valve currently hooks almost everything within the memory sector. however they dont even attempt to touch these. \
HOWEVER, as we're using them as exports, if you open the DLL within IDA you'll be able to see the calls and more than likely it will assist reverse engineers. \
To avoid this, simply use something such as lazy importer (https://github.com/JustasMasiulis/lazy_importer)

void NTFunctions::SetupNTFunctions() {//if u want to add a NTFunction, do it within this!
	auto ntHandle = GetModuleHandle("ntdll.dll");

	NtProtectVirtualMemory = (tNtProtectVirtualMemory)GetProcAddress(ntHandle, "NtProtectVirtualMemory");
	NtQueryVirtualMemory = (tNtQueryVirtualMemory)GetProcAddress(ntHandle, "NtQueryVirtualMemory");
}