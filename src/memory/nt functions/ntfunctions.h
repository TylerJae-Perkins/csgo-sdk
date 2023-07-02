#include <Windows.h>

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation, // MEMORY_BASIC_INFORMATION
	MemoryWorkingSetInformation, // MEMORY_WORKING_SET_INFORMATION
	MemoryMappedFilenameInformation, // UNICODE_STRING
	MemoryRegionInformation, // MEMORY_REGION_INFORMATION
	MemoryWorkingSetExInformation, // MEMORY_WORKING_SET_EX_INFORMATION
	MemorySharedCommitInformation, // MEMORY_SHARED_COMMIT_INFORMATION
	MemoryImageInformation, // MEMORY_IMAGE_INFORMATION
	MemoryRegionInformationEx, // MEMORY_REGION_INFORMATION
	MemoryPrivilegedBasicInformation,
	MemoryEnclaveImageInformation, // MEMORY_ENCLAVE_IMAGE_INFORMATION // since REDSTONE3
	MemoryBasicInformationCapped, // 10
	MemoryPhysicalContiguityInformation, // MEMORY_PHYSICAL_CONTIGUITY_INFORMATION // since 20H1
	MaxMemoryInfoClass
} MEMORY_INFORMATION_CLASS;

typedef long(NTAPI* tNtProtectVirtualMemory) (HANDLE ProcessHandle, PVOID* BaseAddress, ULONG* NumberOfBytesToProtect, ULONG NewAccessProtection, ULONG* OldAccessProtection);
typedef long(NTAPI* tNtQueryVirtualMemory)   (HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);

//Use these as Valve currently hooks almost everything within the memory sector. however they dont even attempt to touch these. \
HOWEVER, as we're using them as exports, if you open the DLL within IDA you'll be able to see the calls and more than likely it will assist reverse engineers. \
To avoid this, simply use something such as lazy importer (https://github.com/JustasMasiulis/lazy_importer)
namespace NTFunctions
{
	inline tNtProtectVirtualMemory NtProtectVirtualMemory;
	inline tNtQueryVirtualMemory NtQueryVirtualMemory;

	void SetupNTFunctions();//if u want to add a NTFunction, do it within this!
}