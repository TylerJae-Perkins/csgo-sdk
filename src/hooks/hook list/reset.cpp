#include "../hooks.h"
#include "../../gui/gui.h"

long D3DAPI Hooks::hkReset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
    static auto oReset = VirtualTables::m_pDirectDevice->GetOriginal< decltype( &Hooks::hkReset ) >( EVTableIndex::RESET );

    // check for first initialization
    if ( !gui::m_bInitialized )
        return oReset( pDevice, pPresentationParameters );

    // invalidate vertex & index buffer, release fonts texture
    ImGui_ImplDX9_InvalidateDeviceObjects( );

    const HRESULT hReset = oReset( pDevice, pPresentationParameters );

    // get directx device and create fonts texture
    if ( hReset == D3D_OK )
        ImGui_ImplDX9_CreateDeviceObjects( );

    return hReset;
}