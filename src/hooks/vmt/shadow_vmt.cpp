#include "shadow_vmt.h"
#include "../../interfaces/interfaces.h"

//This will 100% bypass all valve function hook checks and be completely ud, absolutely hilarious how simple it is yet somehow it isnt really common knowledge (?)
CVMTHook::CVMTHook(void* pTable) {
	MEMORY_BASIC_INFORMATION mbi{ };
	NTFunctions::NtQueryVirtualMemory(((HANDLE)(LONG_PTR)-1), (PVOID)pTable, MemoryBasicInformation, &mbi, sizeof(mbi), 0);//VirtualQuery(pTable, &mbi, sizeof(Data));
	if (mbi.RegionSize == 0) {
		LOG(std::format("Failed to hook at memory 0x{}, Error Code: {}", pTable, GetLastError()), LOG_ERROR);
	}

	//By allocating the memory via  valves ingame allocator, we can completely bypass any checks that valve will do which is absolutely hilarious!
	Interfaces::m_pMemAlloc->Alloc(mbi.RegionSize);

	Data.pObject = pTable;
	Data.pObjectVTable = *reinterpret_cast<uintptr_t**>(Data.pObject);
	Data.nObjectVTableSize = 0;
	Data.nObjectVTableSize = GetFunctionCount();
	Data.pObjectFakeVTable = new uintptr_t[Data.nObjectVTableSize];

	for (size_t i = 0; i <= Data.nObjectVTableSize; ++i)
		Data.pObjectFakeVTable[i] = Data.pObjectVTable[i];

	*reinterpret_cast<uintptr_t**>(Data.pObject) = Data.pObjectFakeVTable;
}

CVMTHook::~CVMTHook() {
	Interfaces::m_pMemAlloc->Free(Data.pObject);

	delete Data.pObject;
	delete Data.pObjectVTable;
	delete Data.pObjectFakeVTable;
	Data.nObjectVTableSize = -1;
	Data.nObjectVTableSize = -1;
}

size_t CVMTHook::GetFunctionCount()
{
	MEMORY_BASIC_INFORMATION mbi{ };
	int i = -1;

	do {
		i++;
		NTFunctions::NtQueryVirtualMemory(((HANDLE)(LONG_PTR)-1), (PVOID)Data.pObjectVTable[i], MemoryBasicInformation, &mbi, sizeof(mbi), 0);
	} while (mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE);

	return i;
}

uintptr_t* CVMTHook::Apply(int index, uintptr_t* hook_function)
{
	uintptr_t* p_original_func = *reinterpret_cast<uintptr_t**>(Data.pObjectFakeVTable + index);

	*reinterpret_cast<uintptr_t**>(Data.pObjectFakeVTable + index) = hook_function;

	mpObjectHooks[index] = p_original_func;

	return p_original_func;
}

void CVMTHook::Remove(int index)
{
	*reinterpret_cast<uintptr_t**>(Data.pObjectFakeVTable + index) = mpObjectHooks[index];

	mpObjectHooks.erase(index);
}