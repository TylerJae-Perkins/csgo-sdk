#include "../Hooks.h"

void __fastcall Hooks::hkFrameStageNotify( IBaseClientDll* thisptr, int edx, EClientFrameStage stage ) {
	static auto oFSN = VirtualTables::m_pClientDll->GetOriginal<decltype( &Hooks::hkFrameStageNotify )>( EVTableIndex::FRAMESTAGENOTIFY );
	return oFSN( thisptr, edx, stage );
}