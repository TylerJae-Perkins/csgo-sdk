#include "../hooks.h"
#include "../../gui/gui.h"

long D3DAPI Hooks::hkEndScene( IDirect3DDevice9* pDevice )
{
    static auto  oEndScene = VirtualTables::m_pDirectDevice->GetOriginal< decltype( &Hooks::hkEndScene ) >( EVTableIndex::ENDSCENE );
    static void* pUsedAddress = nullptr;

    if ( pUsedAddress == nullptr )
    {
        // search for gameoverlay address
        MEMORY_BASIC_INFORMATION memInfo;
        NTFunctions::NtQueryVirtualMemory(((HANDLE)(LONG_PTR)-1), (PVOID)_ReturnAddress(), MemoryBasicInformation, &memInfo, sizeof(memInfo), 0);
      
        char chModuleName[ MAX_PATH ];
        GetModuleFileName( static_cast< HMODULE >( memInfo.AllocationBase ), chModuleName, MAX_PATH );

        if ( strstr( chModuleName, GAMEOVERLAYRENDERER_DLL ) != nullptr )
            pUsedAddress = _ReturnAddress( );
    }

    // check for called from gameoverlay and render here to bypass capturing programs
    if ( _ReturnAddress( ) == pUsedAddress )
    {
        // init gui (fonts, sizes, styles, colors) once
        if ( !gui::m_bInitialized )
            gui::Initialize( pDevice );

        ImGui_ImplDX9_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        // render cheat menu & visuals
        gui::RenderGui( pDevice );

        ImGui::EndFrame( );
        ImGui::Render( );

        // render draw lists from draw data
        ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
    }

    return oEndScene( pDevice );
}