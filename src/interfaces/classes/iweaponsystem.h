#pragma once
#include "../../memory/memory.h"

class IWeaponSystem
{
public:
	CCSWeaponData* GetWeaponData( short nItemDefinitionIndex )
	{
		return Memory::CallVFunc<CCSWeaponData*>( this, 2, nItemDefinitionIndex );
	}
};