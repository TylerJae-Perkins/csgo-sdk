#include <windows.h>
#include <map>

#include "../../memory/nt functions/ntfunctions.h"

struct VMTData
{
	void* pObject;
	uintptr_t* pObjectVTable;
	uintptr_t* pObjectFakeVTable;
	size_t					  nObjectVTableSize;
};

class CVMTHook
{
private:
	VMTData Data;
	std::map<int, uintptr_t*> mpObjectHooks;

	size_t GetFunctionCount();
public:
	CVMTHook() = default;

	CVMTHook(void* pTable);

	~CVMTHook();

	uintptr_t* Apply(int index, uintptr_t* hook_function);

	void Remove(int index);

	template<typename T>
	T GetOriginal(int i)
	{
		return (T)Data.pObjectVTable[i];
	}

};