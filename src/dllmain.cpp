#include "common.h"
#include "setup/setup.h"
#include "memory/nt functions/ntfunctions.h"
#include "utilities/inputhandler.h"
#include "config/variables.h"

DWORD WINAPI OnDllAttach( LPVOID lpParameter )
{
    try
    {
        Setup::OnStartup( );
    }
    catch ( const std::exception& ex )
    {
        // print error message
        LOG(ex.what(), LOG_ERROR);

#ifdef _DEBUG
        // show error message window (or replace to your exception handler)
        _RPT0( _CRT_ERROR, ex.what( ) );
#else
        // unload
        FreeLibraryAndExitThread( static_cast< HMODULE >( lpParameter ), EXIT_FAILURE );
#endif
    }

    return 1UL;
}

DWORD WINAPI OnDllDetach( LPVOID lpParameter )
{
    // unload cheat if pressed specified key
    while ( !InputHandler::HandleInput( Variables::m_iUnloadKey ) )
        std::this_thread::sleep_for( 500ms );

    Setup::OnShutdown( );

    // free our library memory from process and exit from our thread
    FreeLibraryAndExitThread( ( HMODULE )lpParameter, EXIT_SUCCESS );
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
    if ( dwReason == DLL_PROCESS_ATTACH )
    {
        // disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
        DisableThreadLibraryCalls( hModule );

        // save our module
        Globals::m_hDll = hModule;

        // create main thread
        if ( auto hThread = CreateThread( nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr ); hThread != nullptr )
            CloseHandle( hThread );

        // create detach thread
        if ( auto hThread = CreateThread( nullptr, 0U, OnDllDetach, hModule, 0UL, nullptr ); hThread != nullptr )
            CloseHandle( hThread );

        return TRUE;
    }

    return FALSE;
}