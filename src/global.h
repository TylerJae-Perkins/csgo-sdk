#pragma once
#include "valve/entity.h"
#include "valve/usercmd.h"

namespace Globals
{
	// current module
	inline HMODULE			m_hDll = nullptr;
	// unloading?
	inline bool				m_bIsUnloading = false;

	// local player pointer
	//inline CBaseEntity*		m_pLocalPlayer = nullptr;
	// current user command
	inline CUserCmd*		m_pUserCmd = nullptr;
}