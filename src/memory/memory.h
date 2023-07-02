#pragma once
#include "../common.h"
#include "pe32.h"

#pragma region memory_modules_definitions
#define ENGINE_DLL				XorStr("engine.dll")
#define CLIENT_DLL				XorStr("client.dll")
#define LOCALIZE_DLL			XorStr("localize.dll")
#define MATERIALSYSTEM_DLL		XorStr("materialsystem.dll")
#define VGUI_DLL				XorStr("vguimatsurface.dll")
#define VGUI2_DLL				XorStr("vgui2.dll")
#define SHADERPIDX9_DLL			XorStr("shaderapidx9.dll")
#define GAMEOVERLAYRENDERER_DLL XorStr("gameoverlayrenderer.dll")
#define PHYSICS_DLL				XorStr("vphysics.dll")
#define VSTDLIB_DLL				XorStr("vstdlib.dll")
#define TIER0_DLL				XorStr("tier0.dll")
#define INPUTSYSTEM_DLL			XorStr("inputsystem.dll")
#define STUDIORENDER_DLL		XorStr("studiorender.dll")
#define DATACACHE_DLL			XorStr("datacache.dll")
#define STEAM_API_DLL			XorStr("steam_api.dll")
#define MATCHMAKING_DLL			XorStr("matchmaking.dll")
#define SERVER_DLL				XorStr("server.dll")
#define SERVERBROWSER_DLL		XorStr("serverbrowser.dll")
#define FILESYSTEM_DLL			XorStr("filesystem_stdio.dll")
#define PANORAMA_DLL			XorStr("panorama.dll")
#pragma endregion

template <typename T>
class CSingleton
{
protected:
	CSingleton( )
	{
	}

	~CSingleton( )
	{
	}

	CSingleton( const CSingleton& ) = delete;
	CSingleton& operator=( const CSingleton& ) = delete;

	CSingleton( CSingleton&& ) = delete;
	CSingleton& operator=( CSingleton&& ) = delete;

public:
	static T& Get( )
	{
		static T pInstance{};
		return pInstance;
	}
};

// @Note: Added this in for now, yoinked from qo0. Replace in the future if you don't want to use this.
namespace Memory
{
	/// ida style pattern byte comparison in specific module
	/// @param szModuleName : module name where search for pattern
	/// @param szPattern : ida style pattern, e.g. "55 8B 40 ? 30", wildcard can be either '?' or "??", blank delimiters are ignored
	std::uintptr_t FindPattern( const std::string_view szModuleName, const std::string_view szPattern );
	/// ida style pattern byte comparison in specific region
	/// @param szPattern : ida style pattern, e.g. "55 8B 40 ? 30", wildcard can be either '?' or "??", blank delimiters are ignored
	std::uintptr_t FindPattern( const std::uint8_t* uRegionStart, const std::uintptr_t uRegionSize, const std::string_view szPattern );
	// convert pattern to bytes
	std::vector<std::optional<std::uint8_t>> PatternToBytes( const std::string_view szPattern );
	/// @param szModuleName : module name to search base handle for, empty means current module
	/// @returns : base handle of given module name
	void* GetModuleBaseHandle( const std::string_view szModuleName );
	/// alternative of 'GetProcAddress()'
	/// @returns : export procedure address
	void* GetExportAddress( const void* pModuleBase, const std::string_view szProcedureName );
	/// @returns : absolute address from relative address
	inline std::uintptr_t GetAbsoluteAddress( const std::uintptr_t uRelativeAddress )
	{
		return uRelativeAddress + 0x4 + *reinterpret_cast< std::int32_t* >( uRelativeAddress );
	}
	/// @returns : virtual function pointer of specified class at given index
	template <typename T = void*>
	constexpr T GetVFunc( void* thisptr, std::size_t nIndex )
	{
		return ( *static_cast< T** >( thisptr ) )[ nIndex ];
	}
	// call virtual function of specified class at given index
	// @note: references should be wrapped with std::ref call!
	template <typename T, typename ... Args_t>
	constexpr T CallVFunc( void* thisptr, std::size_t nIndex, Args_t... argList )
	{
		using VirtualFn = T( __thiscall* )( void*, decltype( argList )... );
		return ( *static_cast< VirtualFn** >( thisptr ) )[ nIndex ]( thisptr, argList... );
	}
}