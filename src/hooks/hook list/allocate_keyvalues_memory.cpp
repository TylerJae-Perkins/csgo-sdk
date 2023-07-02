#include "../hooks.h"

void* __fastcall Hooks::hkAllocKeyValuesMemory( IKeyValuesSystem* thisptr, int edx, int iSize )
{
    static auto oAllocKeyValuesMemory = VirtualTables::m_pKeyValuesSystem->GetOriginal< decltype( &Hooks::hkAllocKeyValuesMemory ) >( EVTableIndex::ALLOCKEYVALUESMEMORY );

    // return addresses of check function
    // @credits: danielkrupinski
    static const std::uintptr_t uAllocKeyValuesEngine = Memory::GetAbsoluteAddress( Memory::FindPattern( ENGINE_DLL, XorStr( "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ) ) + 0x1 ) + 0x4A;
    static const std::uintptr_t uAllocKeyValuesClient = Memory::GetAbsoluteAddress( Memory::FindPattern( CLIENT_DLL, XorStr( "E8 ? ? ? ? 83 C4 08 84 C0 75 10" ) ) + 0x1 ) + 0x3E;

    if ( const std::uintptr_t uReturnAddress = reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ); uReturnAddress == uAllocKeyValuesEngine || uReturnAddress == uAllocKeyValuesClient )
        return nullptr;

    return oAllocKeyValuesMemory( thisptr, edx, iSize );
}