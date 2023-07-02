#pragma once
#include "../../memory/memory.h"

class IVPanel
{
public:
	void SetKeyBoardInputEnabled( unsigned int nPanelIndex, bool bState )
	{
		return Memory::CallVFunc<void>( this, 31, nPanelIndex, bState );
	}

	void SetMouseInputEnabled( unsigned int nPanelIndex, bool bState )
	{
		return Memory::CallVFunc<void>( this, 32, nPanelIndex, bState );
	}

	const char* GetName( int nPanelIndex )
	{
		return Memory::CallVFunc<const char*>( this, 36, nPanelIndex );
	}

	const char* GetNameClass( int nPanelIndex )
	{
		return Memory::CallVFunc<const char*>( this, 37, nPanelIndex );
	}
};