#pragma once
#include <intrin.h>

#include "../common.h"
#include "../Interfaces/Interfaces.h"
#include "vmt/shadow_vmt.h"

enum EVTableIndex : int
{
    /* directx table */
    RESET = 16,
    ENDSCENE = 42,

    /* keyvaluessystem table */
    ALLOCKEYVALUESMEMORY = 2,

    /* client table */
    CREATEMOVEPROXY = 22,
    FRAMESTAGENOTIFY = 37
};

namespace VirtualTables
{
    inline std::unique_ptr<CVMTHook> m_pDirectDevice;
    
    inline std::unique_ptr<CVMTHook> m_pKeyValuesSystem;
   
    inline std::unique_ptr<CVMTHook> m_pClientDll;
}

namespace Hooks
{
    // handlers
    void SetupHooks();
    void OnShutdown();

    long CALLBACK           hkWndProc( HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam );

    /* directx table */
    long D3DAPI             hkReset( IDirect3DDevice9* m_pDevice, D3DPRESENT_PARAMETERS* m_pPresentationParameters );
    long D3DAPI             hkEndScene( IDirect3DDevice9* m_pDevice );

    // keyvalues table
    void* __fastcall        hkAllocKeyValuesMemory( IKeyValuesSystem* thisptr, int edx, int m_iSize );

    //client table
    void __fastcall         hkFrameStageNotify( IBaseClientDll* thisptr, int edx, EClientFrameStage m_stage );
}