#include "Interfaces.h"

// @TODO: Make a handler to automatically check all Interfaces and return false where shit went wrong
bool Interfaces::Setup( )
{
	m_pClientDll =			Capture<IBaseClientDll>( CLIENT_DLL, XorStr( "VClient" ) );
	m_pClientEntityList =	Capture<IClientEntityList>( CLIENT_DLL, XorStr( "VClientEntityList" ) );
	m_pEffects =			Capture<IEffects>( CLIENT_DLL, XorStr( "IEffects" ) );
	m_pGameMovement =		Capture<IGameMovement>( CLIENT_DLL, XorStr( "GameMovement" ) );
	m_pPrediction =			Capture<IPrediction>( CLIENT_DLL, XorStr( "VClientPrediction" ) );
	m_pGameConsole =		Capture<IGameConsole>( CLIENT_DLL, XorStr( "GameConsole" ) );
	m_pGameUI =				Capture<IGameUI>( CLIENT_DLL, XorStr( "GameUI" ) );
	m_pEngine =				Capture<IEngineClient>( ENGINE_DLL, XorStr( "VEngineClient" ) );
	m_pEngineVGui =			Capture<IEngineVGui>( ENGINE_DLL, XorStr( "VEngineVGui" ) );
	m_pEngineTrace =		Capture<IEngineTrace>( ENGINE_DLL, XorStr( "EngineTraceClient" ) );
	m_pEngineSound =		Capture<IEngineSound>( ENGINE_DLL, XorStr( "IEngineSoundClient" ) );
	m_pNetworkContainer =	Capture<INetworkContainer>( ENGINE_DLL, XorStr( "VEngineClientStringTable" ) );
	m_pGameEvent =			Capture<IGameEventManager2>( ENGINE_DLL, XorStr( "GAMEEVENTSMANAGER002" ) );
	m_pRenderView =			Capture<IVRenderView>( ENGINE_DLL, XorStr( "VEngineRenderView" ) );
	m_pDebugOverlay =		Capture<IVDebugOverlay>( ENGINE_DLL, XorStr( "VDebugOverlay" ) );
	m_pEngineEffects =		Capture<IVEngineEffects>( ENGINE_DLL, XorStr( "VEngineEffects" ) );
	m_pModelInfo =			Capture<IVModelInfo>( ENGINE_DLL, XorStr( "VModelInfoClient" ) );
	m_pModelRender =		Capture<IVModelRender>( ENGINE_DLL, XorStr( "VEngineModel" ) );
	m_pMDLCache =			Capture<IMDLCache>( DATACACHE_DLL, XorStr( "MDLCache" ) );
	m_pStudioRender =		Capture<IStudioRender>( STUDIORENDER_DLL, XorStr( "VStudioRender" ) );
	m_pConVar =				Capture<IConVar>( VSTDLIB_DLL, XorStr( "VEngineCvar" ) );
	m_pPhysicsProps =		Capture<IPhysicsSurfaceProps>( PHYSICS_DLL, XorStr( "VPhysicsSurfaceProps" ) );
	m_pMaterialSystem =		Capture<IMaterialSystem>( MATERIALSYSTEM_DLL, XorStr( "VMaterialSystem" ) );
	m_pSurface =			Capture<ISurface>( VGUI_DLL, XorStr( "VGUI_Surface" ) );
	m_pPanel =				Capture<IVPanel>( VGUI2_DLL, XorStr( "VGUI_Panel" ) );
	m_pInputSystem =		Capture<IInputSystem>( INPUTSYSTEM_DLL, XorStr( "InputSystemVersion" ) );
	m_pLocalize =			Capture<ILocalize>( LOCALIZE_DLL, XorStr( "Localize_" ) );
	m_pMatchFramework =		Capture<IMatchFramework>( MATCHMAKING_DLL, XorStr( "MATCHFRAMEWORK_" ) );
	m_pGameTypes =			Capture<IGameTypes>( MATCHMAKING_DLL, XorStr( "VENGINE_GAMETYPES_VERSION" ) );
	m_pServer =				Capture<IServerGameDLL>( SERVER_DLL, XorStr( "ServerGameDLL" ) );
	m_pFileSystem =			Capture<IBaseFileSystem>( FILESYSTEM_DLL, XorStr( "VBaseFileSystem" ) );

	m_pGlobalVariables = **reinterpret_cast< IGlobalVars*** >( ( *reinterpret_cast< DWORD** >( m_pClientDll ) )[ 0 ] + 0x1F );
	if ( m_pGlobalVariables == nullptr )
		return false;

	m_pClientMode = **reinterpret_cast< IClientModeShared*** >( Memory::GetVFunc<std::uintptr_t>( m_pClientDll, 10 ) + 0x5 ); // get it from CHLClient::HudProcessInput
	if ( m_pClientMode == nullptr )
		return false;

	m_pKeyValuesSystem = reinterpret_cast< KeyValuesSystemFn >( Memory::GetExportAddress( Memory::GetModuleBaseHandle( VSTDLIB_DLL ), XorStr( "KeyValuesSystem" ) ) )( );
	if ( m_pKeyValuesSystem == nullptr )
		return false;

	m_pDirectDevice = **reinterpret_cast< IDirect3DDevice9*** >( Memory::FindPattern( SHADERPIDX9_DLL, XorStr( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ) + 0x1 );
	if ( m_pDirectDevice == nullptr )
		return false;

	m_pMemAlloc = *static_cast< IMemAlloc** >( Memory::GetExportAddress( Memory::GetModuleBaseHandle( TIER0_DLL ), XorStr( "g_pMemAlloc" ) ) );
	if ( m_pMemAlloc == nullptr )
		return false;

	m_pWeaponSystem = *reinterpret_cast< IWeaponSystem** >( Memory::FindPattern( CLIENT_DLL, XorStr( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2 );
	if ( m_pWeaponSystem == nullptr )
		return false;

	return true;
}

template <typename T>
T* Interfaces::Capture( const std::string_view szModuleName, const std::string_view szInterface )
{
	const auto GetRegisterList = [ &szModuleName ]( ) -> CInterfaceRegister*
	{
		void* oCreateInterface = nullptr;

		if ( const auto hModule = Memory::GetModuleBaseHandle( szModuleName ); hModule != nullptr )
			oCreateInterface = Memory::GetExportAddress( hModule, XorStr( "CreateInterface" ) );

		if ( oCreateInterface == nullptr )
			throw std::runtime_error( XorStr( "failed get createinterface address" ) );

		const std::uintptr_t uCreateInterfaceRelative = reinterpret_cast< std::uintptr_t >( oCreateInterface ) + 0x5;
		const std::uintptr_t uCreateInterface = uCreateInterfaceRelative + 4U + *reinterpret_cast< std::int32_t* >( uCreateInterfaceRelative );
		return **reinterpret_cast< CInterfaceRegister*** >( uCreateInterface + 0x6 );
	};

	for ( auto pRegister = GetRegisterList( ); pRegister != nullptr; pRegister = pRegister->m_pNext )
	{
		// found needed interface
		if ( ( std::string_view( pRegister->m_szName ).compare( 0U, szInterface.length( ), szInterface ) == 0 &&
			// and it have digits after name
			std::atoi( pRegister->m_szName + szInterface.length( ) ) > 0 ) ||
			// or given full name with hardcoded digits
			szInterface.compare( pRegister->m_szName ) == 0 )
		{
			// capture our interface
			auto pInterface = pRegister->m_pCreateFn( );

			return static_cast< T* >( pInterface );
		}
	}

#ifdef DEBUG_CONSOLE
	LOG( std::format("[error] failed to find interface \"{}\" in \"{}\"", szInterface, szModuleName), LOG_ERROR);
#endif

	return nullptr;
}