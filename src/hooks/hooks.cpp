#include "hooks.h"

void Hooks::SetupHooks() 
{
	VirtualTables::m_pDirectDevice = std::make_unique<CVMTHook>(Interfaces::m_pDirectDevice);
	VirtualTables::m_pDirectDevice->Apply( EVTableIndex::RESET, reinterpret_cast< uintptr_t* >( hkReset ) );
	VirtualTables::m_pDirectDevice->Apply( EVTableIndex::ENDSCENE, reinterpret_cast< uintptr_t* >( hkEndScene ) );

	VirtualTables::m_pClientDll = std::make_unique<CVMTHook>(Interfaces::m_pClientDll);
	VirtualTables::m_pClientDll->Apply( EVTableIndex::FRAMESTAGENOTIFY, reinterpret_cast<uintptr_t*>(hkFrameStageNotify));

	VirtualTables::m_pKeyValuesSystem = std::make_unique<CVMTHook>(Interfaces::m_pKeyValuesSystem);
	VirtualTables::m_pKeyValuesSystem->Apply( EVTableIndex::ALLOCKEYVALUESMEMORY, reinterpret_cast< uintptr_t* >( hkAllocKeyValuesMemory ) );
}

void Hooks::OnShutdown() 
{
	VirtualTables::m_pDirectDevice->Remove( EVTableIndex::RESET );
	VirtualTables::m_pDirectDevice->Remove( EVTableIndex::ENDSCENE );
	VirtualTables::m_pDirectDevice->~CVMTHook();

	VirtualTables::m_pClientDll->Remove( EVTableIndex::FRAMESTAGENOTIFY );
	VirtualTables::m_pDirectDevice->~CVMTHook();

	VirtualTables::m_pKeyValuesSystem->Remove( EVTableIndex::ALLOCKEYVALUESMEMORY );
	VirtualTables::m_pDirectDevice->~CVMTHook();
}