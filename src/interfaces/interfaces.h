#pragma once
#include "../common.h"

// class includes
#include "classes/ibaseclientdll.h"
#include "classes/icliententitylist.h"
#include "classes/isurface.h"
#include "classes/iclientmodeshared.h"
#include "classes/iconvar.h"
#include "classes/ieffects.h"
#include "classes/inetchannel.h"
#include "classes/iengineclient.h"
#include "classes/ienginetrace.h"
#include "classes/ienginevgui.h"
#include "classes/ienginesound.h"
#include "classes/igameconsole.h"
#include "classes/igameeventmanager.h"
#include "classes/ivpanel.h"
#include "classes/igameui.h"
#include "classes/iglobalvars.h"
#include "classes/iservergamedll.h"
#include "classes/iinputsystem.h"
#include "classes/ilocalize.h"
#include "classes/imatchframework.h"
#include "classes/igametypes.h"
#include "classes/imaterialsystem.h"
#include "classes/imdlcache.h"
#include "classes/inetworkstring.h"
#include "classes/iphysicssurfaceprops.h"
#include "classes/iprediction.h"
#include "classes/istudiorender.h"
#include "classes/ivdebugoverlay.h"
#include "classes/ivengineeffects.h"
#include "classes/ivrenderview.h"
#include "classes/ivmodelrender.h"
#include "classes/ivmodelinfo.h"
#include "classes/ikeyvaluessystem.h"
#include "classes/imemalloc.h"
#include "classes/iweaponsystem.h"

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	m_pCreateFn;		//0x0
	const char*				m_szName;			//0x4
	CInterfaceRegister*		m_pNext;			//0x8
};

namespace Interfaces
{
	bool Setup( );
	/* search and returns a pointer of specific interface given */
	template <typename T>
	T* Capture( const std::string_view szModuleName, const std::string_view szInterface );

	// Interfaces
	inline IBaseClientDll*			m_pClientDll			= nullptr;
	inline IClientEntityList*		m_pClientEntityList		= nullptr;
	inline IEffects*				m_pEffects				= nullptr;
	inline IGameMovement*			m_pGameMovement			= nullptr;
	inline IMoveHelper*				m_pMoveHelper			= nullptr;
	inline IPrediction*				m_pPrediction			= nullptr;
	inline IGameConsole*			m_pGameConsole			= nullptr;
	inline IGameUI*					m_pGameUI				= nullptr;
	inline IEngineClient*			m_pEngine				= nullptr;
	inline IEngineVGui*				m_pEngineVGui			= nullptr;
	inline IEngineTrace*			m_pEngineTrace			= nullptr;
	inline IEngineSound*			m_pEngineSound			= nullptr;
	inline INetworkContainer*		m_pNetworkContainer		= nullptr;
	inline IGameEventManager2*		m_pGameEvent			= nullptr;
	inline IVRenderView*			m_pRenderView			= nullptr;
	inline IVDebugOverlay*			m_pDebugOverlay			= nullptr;
	inline IVEngineEffects*			m_pEngineEffects		= nullptr;
	inline IVModelInfo*				m_pModelInfo			= nullptr;
	inline IVModelRender*			m_pModelRender			= nullptr;
	inline IMDLCache*				m_pMDLCache				= nullptr;
	inline IStudioRender*			m_pStudioRender			= nullptr;
	inline IConVar*					m_pConVar				= nullptr;
	inline IPhysicsSurfaceProps*	m_pPhysicsProps			= nullptr;
	inline IMaterialSystem*			m_pMaterialSystem		= nullptr;
	inline ISurface*				m_pSurface				= nullptr;
	inline IVPanel*					m_pPanel				= nullptr;
	inline IInputSystem*			m_pInputSystem			= nullptr;
	inline ILocalize*				m_pLocalize				= nullptr;
	inline IMatchFramework*			m_pMatchFramework		= nullptr;
	inline IGameTypes*				m_pGameTypes			= nullptr;
	inline IServerGameDLL*			m_pServer				= nullptr;
	inline IBaseFileSystem*			m_pFileSystem			= nullptr;

	inline IGlobalVarsBase*			m_pGlobalVariables		= nullptr;
	inline IClientModeShared*		m_pClientMode			= nullptr;
	inline IKeyValuesSystem*		m_pKeyValuesSystem		= nullptr;
	inline IDirect3DDevice9*		m_pDirectDevice			= nullptr;
	inline IMemAlloc*				m_pMemAlloc				= nullptr;
	inline IWeaponSystem*			m_pWeaponSystem			= nullptr;
}